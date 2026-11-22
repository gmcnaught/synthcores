#!/usr/bin/env bash
# Build the monorepo from the three core repos, keeping each one's full history under cores/<name>/.
# It reads the source repos and never modifies them. It writes only to OUT.
#
#   scripts/import-cores.sh OUT_DIR
#
# Needs git-filter-repo: pip install git-filter-repo, or brew install git-filter-repo.
# Trial run on 2026-11-22: 376 commits imported; git log and git blame on cores/*/ paths still work.
set -euo pipefail

OUT=${1:?usage: import-cores.sh OUT_DIR}
SRC=$(cd "$(dirname "$0")/.." && pwd)
FR=${GIT_FILTER_REPO:-git-filter-repo}
command -v "$FR" >/dev/null || { echo "git-filter-repo not found (set GIT_FILTER_REPO)" >&2; exit 1; }
[ -e "$OUT" ] && { echo "$OUT exists; refusing to overwrite" >&2; exit 1; }

# One line per core: source repo, name under cores/, branch to import.
CORES=(
  "nuked-sc55             sc55        main"
  "synclavier-ii-MiSTer   synclavier  main"
  "emu-proteus-2k-MiSTer  proteus     p2k-research"
)

TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT

for line in "${CORES[@]}"; do
  read -r repo name branch <<<"$line"
  if [ -n "$(git -C "$SRC/$repo" status --porcelain --untracked-files=no)" ]; then
    echo "warning: $repo has uncommitted changes; only committed history on $branch is imported" >&2
  fi
  git clone -q --no-local --branch "$branch" "$SRC/$repo" "$TMP/$name"
  (cd "$TMP/$name" && "$FR" --quiet --force \
      --to-subdirectory-filter "cores/$name" --tag-rename ":$name-" --refs "$branch")
done

git init -q -b main "$OUT"
cd "$OUT"
git commit -q --allow-empty -m "Create monorepo"
for line in "${CORES[@]}"; do
  read -r repo name branch <<<"$line"
  git fetch -q "$TMP/$name" "$branch"
  git merge -q --allow-unrelated-histories --no-edit -m "Import $name history from $repo ($branch)" FETCH_HEAD
done

echo "Imported $(git rev-list --count HEAD) commits into $OUT"
echo "Next: move cores/sc55/third_party/nuked-sc55 (a submodule gitlink) to modules/nuked-sc55 with a root .gitmodules."
