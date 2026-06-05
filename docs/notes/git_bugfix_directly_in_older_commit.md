# Git History Surgery: Folding Bug Fixes into an Older Commit

## Goal

I had an older commit:

```text

aa40702  Refactor packing/unpacking logic...

```

that contained multiple bugs.

I wanted the final history to look as if the bugs had been fixed when that commit was originally written.

Desired result:

```text

A  B'  C'  D'  E'  F'

```

instead of:

```text

A  B  C  D  E  F  Fix1  Fix2  Fix3

```

---

## Core Lesson

Git commits are immutable.

A commit hash depends on:

- Snapshot (tree)

- Parent hash

- Author/committer metadata

- Commit message

Changing anything creates a new commit.

Therefore:

```text

A -> B -> C -> D

```

becomes:

```text

A -> B' -> C' -> D'

```

when `B` is modified.

Every descendant commit must be rewritten.

---

# Correct Workflow

## 1. Create Fixup Commits

After fixing bugs:

```bash

git add <files>

git commit --fixup <target_commit>

```

Example:

```bash

git commit --fixup aa40702

```

Repeat for every bug fix.

---

## 2. Start Interactive Rebase

```bash

git rebase -i aa40702~1

```

---

## 3. Edit `git-rebase-todo`

The source of truth is **not the GUI**.

The source of truth is the actual `git-rebase-todo` file.

Initial state:

```text

pick aa40702

pick fix1

pick fix2

pick fix3

pick later_commit_1

pick later_commit_2

...

```

Desired state:

```text

pick aa40702

squash fix1

squash fix2

squash fix3

pick later_commit_1

pick later_commit_2

...

```

### Important

The fix commits must be immediately adjacent to the target commit.

If they are not adjacent, they will not be folded into the target commit.

---

## 4. Edit Commit Message

Git opens a combined commit message.

Example:

```text

Refactor packing/unpacking logic...

Bug fixes:

- Fix memcpy byte count handling

- Fix depadding logic

- Fix temp_key initialization

```

Save and close.

---

## 5. Verify

Check:

```bash

git show <new_commit_hash>

```

Verify:

- Bug fix code exists

- Commit message contains fixes

- Separate bug-fix commits are gone

---

## 6. Push

History changed.

A normal push will fail.

Use:

```bash

git push --force-with-lease origin main

```

---

# Biggest Mistake Today

I created fixup commits while checked out near the old commit.

Result:

```text

main

 |

 +-- later commits

aa40702

 |

 +-- fix1

 +-- fix2

 +-- fix3

```

The fixups ended up on a side branch rather than on top of `main`.

### Solution

Cherry-pick the fixes onto `main` first, then perform the rebase.

---

# Most Valuable Recovery Tool

```bash

git reflog

```

Reflog saved everything.

Useful for:

- Recovering lost commits

- Recovering from rebases

- Recovering from detached HEAD states

- Finding orphaned fixup commits

When things look wrong:

```bash

git reflog

```

before doing anything else.

---

# Git Mental Model

A commit does **not** store a diff.

Git stores:

```text

Commit

  ↓

Tree

  ↓

Blobs

```

Where:

- Commit = metadata + parent + tree pointer

- Tree = directory structure

- Blob = entire file contents

Diffs are computed when needed.

---

# Blobs vs Diffs

A blob is the complete contents of a file.

Example:

```c

int add(int a, int b) {

    return a + b;

}

```

Change one character:

```c

int add(int a, int b) {

    return a + b + 1;

}

```

Git creates a new blob containing the entire file.

Conceptually:

```text

Blob A = complete file

Blob B = complete modified file

```

Not:

```diff

- return a + b;

+ return a + b + 1;

```

---

# Why Repositories Don't Explode in Size

Conceptually:

```text

Commit -> Tree -> Full-file Blobs

```

Physically Git later performs:

- Compression

- Packfiles

- Delta compression

So Git may internally store:

```text

Blob B = Blob A + small delta

```

for efficiency.

### Important Distinction

Logical model:

```text

Snapshots

```

Storage model:

```text

Compressed snapshots with deltas

```

---

# Final Takeaways

1. Trust `git-rebase-todo`, not GUI ordering.

2. Use `git commit --fixup`.

3. Use interactive rebase to rewrite history.

4. Verify rewritten commits before pushing.

5. Use `git reflog` whenever things become confusing.

6. Use `git push --force-with-lease` after history rewrites.

7. Git is fundamentally a snapshot system, not a patch system.

---

## Future Me Checklist

```bash

git add <files>

git commit --fixup <commit>

git rebase -i --autosquash <commit>~1

# Verify

git show <new_commit>

git push --force-with-lease origin main

```

If panic occurs:

```bash

git reflog

```

First. Always.