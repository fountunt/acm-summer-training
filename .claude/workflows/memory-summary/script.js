export const meta = {
  name: 'memory-summary',
  description: '自动扫描、汇总、整理记忆文件（memory/ 目录），更新 MEMORY.md 索引，生成全局摘要',
  phases: [
    { title: '扫描', detail: '列出所有记忆文件' },
    { title: '读取', detail: '逐条读取并归纳' },
    { title: '索引', detail: '更新 MEMORY.md' },
    { title: '摘要', detail: '生成全局概览' },
  ],
}

const MEMORY_DIR = 'C:/Users/QeQ/.claude/projects/C--acm-summer-training/memory'
const INDEX_PATH = MEMORY_DIR + '/MEMORY.md'

// ── Phase 1: 扫描 ──────────────────────────────────────────────
phase('扫描')
const files = await agent(`List all .md files in ${MEMORY_DIR} (excluding MEMORY.md). Output ONLY a valid JSON array of file names, e.g. ["foo.md","bar.md"]. Return [] if none.`, {
  label: 'scan-memory-dir',
  phase: '扫描',
  schema: {
    type: 'object',
    properties: {
      files: { type: 'array', items: { type: 'string' } },
    },
    required: ['files'],
  },
})
const fileList = files?.files ?? []
log(`找到 ${fileList.length} 个记忆文件`)

if (fileList.length === 0) {
  log('记忆目录为空，跳过后续步骤')
  // 确保 MEMORY.md 存在但为空
  phase('索引')
  await agent(`If ${INDEX_PATH} exists, do nothing. Otherwise create it with content "# 记忆索引\\n\\n（暂无记忆）" 作为纯文本。`, {
    label: 'ensure-index',
    phase: '索引',
  })
  phase('摘要')
  return { summary: '没有需要总结的记忆', fileCount: 0 }
}

// ── Phase 2: 逐条读取 ──────────────────────────────────────────
phase('读取')
const summaries = await pipeline(
  fileList,
  async (file) => {
    const raw = await agent(`Read the file "${MEMORY_DIR}/${file}" (it's a Markdown file with frontmatter). Extract and return:
- name (from frontmatter)
- description (from frontmatter)
- type (from frontmatter metadata.type)
- contentSummary (one-sentence summary of the body)
- stale (true if the content feels outdated — e.g. mentions past dates, "last week", "yesterday")
Return null if the file isn't valid Markdown.`, {
      label: `read:${file}`,
      phase: '读取',
      schema: {
        type: 'object',
        properties: {
          name: { type: 'string' },
          description: { type: 'string' },
          type: { type: 'string', enum: ['user', 'feedback', 'project', 'reference'] },
          contentSummary: { type: 'string' },
          stale: { type: 'boolean' },
        },
        required: ['name', 'description', 'type', 'contentSummary', 'stale'],
      },
    })
    return { file, ...raw }
  },
)
const validSummaries = summaries.filter(Boolean)

// ── Phase 3: 更新 MEMORY.md ────────────────────────────────────
phase('索引')

// 读取现有索引
const existingIndex = await agent(`Read the file at "${INDEX_PATH}". If it doesn't exist or is empty, return { lines: [], exists: false }. Otherwise return its lines.`, {
  label: 'read-index',
  phase: '索引',
  schema: {
    type: 'object',
    properties: {
      exists: { type: 'boolean' },
      lines: { type: 'array', items: { type: 'string' } },
    },
    required: ['exists', 'lines'],
  },
})

// 生成新索引条目
const indexEntries = validSummaries.map(s => {
  const desc = s.description || s.contentSummary
  return `- [${s.name}](${s.file}) — ${desc}`
}).join('\n')

let newIndex
if (existingIndex?.exists && existingIndex.lines.length > 0) {
  // 有已有索引，合并更新
  const existingNames = new Set(
    existingIndex.lines
      .filter(l => l.startsWith('- ['))
      .map(l => {
        const m = l.match(/- \[([^\]]+)\]/)
        return m ? m[1] : null
      })
      .filter(Boolean)
  )
  const currentNames = new Set(validSummaries.map(s => s.name))

  // 保留不在当前文件列表中的旧条目（可能是外部创建的）
  const keptLines = existingIndex.lines.filter(l => {
    const m = l.match(/- \[([^\]]+)\]/)
    return !m || currentNames.has(m[1]) // 只保留当前存在的文件的条目
  })

  // 添加新条目
  const newLines = validSummaries
    .filter(s => !existingNames.has(s.name))
    .map(s => `- [${s.name}](${s.file}) — ${s.description || s.contentSummary}`)

  const allLines = [...keptLines, ...newLines]
  newIndex = '# 记忆索引\n\n' + allLines.join('\n') + '\n'
} else {
  newIndex = '# 记忆索引\n\n' + indexEntries + '\n'
}

await agent(`Write the following content to "${INDEX_PATH}" (overwrite if exists):\n\n${newIndex}`, {
  label: 'write-index',
  phase: '索引',
})

log('MEMORY.md 索引已更新')

// ── Phase 4: 全局摘要 ──────────────────────────────────────────
phase('摘要')
const staleOnes = validSummaries.filter(s => s.stale)
const byType = {}
validSummaries.forEach(s => {
  byType[s.type] = (byType[s.type] || 0) + 1
})

await agent(`Write a summary file to "${MEMORY_DIR}/_summary.md" with the following structure (overwrite if exists):

# 记忆全局摘要
> 自动生成于 2026-07-14

## 概览
- 记忆总数：${validSummaries.length}
- 类型分布：${JSON.stringify(byType)}
${staleOnes.length > 0 ? `- ⚠️ 可能过时的记忆：${staleOnes.map(s => s.name).join(', ')}` : '- 暂无过期记忆'}

## 每条摘要
${validSummaries.map(s => `### ${s.name}（${s.file}）
- **描述**：${s.description || '无'}
- **类型**：${s.type}
- **内容**：${s.contentSummary}
- **状态**：${s.stale ? '⚠️ 可能过时' : '✅ 正常'}
`).join('\n')}

## 建议
${staleOnes.length > 0 ? `- 审查以下可能过时的记忆，决定是否需要更新或删除：${staleOnes.map(s => s.name).join('、')}` : '- 所有记忆状态正常'}
- 定期使用此工作流保持索引同步
`, {
  label: 'write-summary',
  phase: '摘要',
})

log(`✅ 记忆总结完成：${validSummaries.length} 条已汇总`)

const typeBreakdown = Object.entries(byType).map(([k, v]) => `${k}: ${v}`).join(', ')
return {
  summary: `共 ${validSummaries.length} 条记忆（${typeBreakdown}）`,
  fileCount: validSummaries.length,
  staleCount: staleOnes.length,
  types: byType,
}
