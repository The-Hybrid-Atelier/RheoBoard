#!/usr/bin/env bash
# Lightweight, dependency-free sanity checks for BuildYourOwn/ (the project's harness/docs home).
#
# This is deliberately not wired into CI — solo repo, no blocking gates (see AGENTS.md).
# It's cheap enough to just run by hand (or have the agent run) before committing, per the
# session wrap-up checklist in AGENTS.md. Exits non-zero if something needs a look.
#
# Checks:
#   1. BuildYourOwn/exec-plans/active/ has at most one plan in flight.
#   2. No dangling relative markdown links under BuildYourOwn/.
#   3. The most recent commit touching BuildYourOwn/ also touched BuildYourOwn/PROGRESS.md.

set -uo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$repo_root"

status=0

echo "== Active exec-plans =="
active_count=$(find BuildYourOwn/exec-plans/active -maxdepth 1 -type f -name '*.md' ! -name 'README.md' 2>/dev/null | wc -l | tr -d ' ')
if [ "$active_count" -gt 1 ]; then
  echo "WARN: $active_count plans in BuildYourOwn/exec-plans/active/ (expected 0-1) — pick one before starting more."
  status=1
else
  echo "OK ($active_count active plan(s))"
fi

echo
echo "== Dangling relative markdown links (BuildYourOwn/) =="
found_broken=0
while IFS= read -r -d '' file; do
  dir=$(dirname "$file")
  while IFS= read -r line; do
    trimmed="$(printf '%s' "$line" | sed -E 's/^[[:space:]]*//')"
    # Skip lines that are an inline code example (start with a backtick) — not real links.
    case "$trimmed" in
      \`*) continue ;;
    esac
    links=$(printf '%s' "$line" | grep -oE '\]\([^)]+\)' || true)
    [ -z "$links" ] && continue
    while IFS= read -r m; do
      link="${m#\](}"
      link="${link%)}"
      case "$link" in
        http://*|https://*|mailto:*|\#*) continue ;;
      esac
      target="${link%%#*}"
      [ -z "$target" ] && continue
      if [ ! -e "$dir/$target" ]; then
        echo "BROKEN: $file -> $link"
        found_broken=1
      fi
    done <<< "$links"
  done < "$file"
done < <(find BuildYourOwn -name '*.md' -print0; printf '%s\0' README.md AGENTS.md)

if [ "$found_broken" -eq 1 ]; then
  status=1
else
  echo "OK (no dangling relative links found)"
fi

echo
echo "== PROGRESS.md freshness =="
last_docs_commit=$(git log -1 --format=%H -- BuildYourOwn 2>/dev/null || true)
last_progress_commit=$(git log -1 --format=%H -- BuildYourOwn/PROGRESS.md 2>/dev/null || true)
if [ -n "$last_docs_commit" ] && [ "$last_docs_commit" != "$last_progress_commit" ]; then
  echo "WARN: most recent commit touching BuildYourOwn/ ($last_docs_commit) didn't also update"
  echo "      BuildYourOwn/PROGRESS.md. Confirm that was intentional."
  status=1
else
  echo "OK"
fi

echo
if [ "$status" -eq 0 ]; then
  echo "All checks passed."
else
  echo "Some checks need attention (see WARN/BROKEN above)."
fi
exit "$status"
