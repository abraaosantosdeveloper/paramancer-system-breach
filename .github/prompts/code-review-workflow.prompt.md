---
name: "Code Review Workflow"
description: "Execute a complete code review workflow: QA validation, issue identification, and developer fix instructions. Use when: reviewing completed features, validating pull requests, or ensuring code quality before merge."
argument-hint: "File(s) to review, feature description, or PR scope"
---

# Code Review Workflow

This workflow combines QA validation with developer-focused fix instructions to ensure high-quality, clean code.

## Workflow Steps

### Step 1: QA Validation
First, I'll use the **QA Engineer** agent to thoroughly review your code for:
- C-specific issues (memory safety, pointers, resource cleanup)
- Clean code violations (SRP, DRY, KISS, naming)
- Raylib pattern compliance
- Error handling and bounds checking
- Performance issues
- Test coverage gaps

### Step 2: Issue Categorization
Issues are organized by severity:
- **CRITICAL**: Must fix before merge
- **HIGH**: Must fix before merge
- **MEDIUM**: Should fix
- **LOW**: Nice to have

### Step 3: Developer Handoff
If issues are found, I'll compile clear, actionable fix instructions for the **Raylib C Developer** agent, including:
- What to fix (specific location and code snippet)
- Why it's wrong
- How to fix it (with examples)

### Step 4: Developer Fixes (Optional)
The developer agent will implement fixes based on QA findings and return clean code ready for re-review.

---

## How to Use This Workflow

### Option A: Review Only (QA Feedback)
**Input**: "Review this file and its issues"
**Output**: Detailed QA report with severity levels

```
/code-review file:actors.c
```

### Option B: Full Review + Fix Cycle
**Input**: "Review this feature and fix any issues found"
**Output**: Clean code with all issues addressed

```
/code-review file:game_loop.c feature:"Main game loop with input, update, render phases"
```

### Option C: Review Multiple Files
**Input**: "Review these files as a feature set"
**Output**: Unified QA report covering all files

```
/code-review files:player.c,enemy.c,collision.c feature:"Player and enemy AI with collision detection"
```

---

## Example Use Cases

### Review a New Feature
```
/code-review 
feature: "Add jump mechanic with gravity"
files: src/player.c
```

### Validate a Complete Module
```
/code-review
feature: "Resource management system"
files: src/assets.c, src/assets.h
detail: "Should validate texture/font loading with error handling"
```

### Full PR Review & Fix
```
/code-review
feature: "Implement enemy patrol AI"
files: src/enemy.c, src/ai.c
fix: true
```

---

## Workflow Output

### QA Phase Output
✅ Code quality checklist  
⚠️ Issues grouped by severity  
📋 Detailed locations and explanations  
🎯 Clear fix instructions for each issue  

### Developer Phase Output (if enabled)
✅ Fixed code  
📝 List of changes made  
🧪 Testing notes  
⚠️ Remaining considerations  

---

## Tips

- **Be specific**: Include file paths and feature descriptions for better context
- **Set expectations**: Are you reviewing for merge readiness, or learning purposes?
- **Use severity**: CRITICAL issues block merge, others are guidance
- **Fix iteratively**: Multiple review cycles improve code quality over time
- **Learn patterns**: Review feedback teaches Raylib best practices for future development

---

## Related Agents

- **QA Engineer**: Validates code quality and finds issues
- **Raylib C Developer**: Implements fixes based on QA feedback
- **Explore**: Investigates codebase context if needed

