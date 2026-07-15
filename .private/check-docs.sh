#!/usr/bin/env bash
# Lightweight, dependency-free sanity checks for BuildYourOwn/ (the project's harness/docs home).
#
# This is deliberately not wired into CI — solo repo, no blocking gates (see AGENTS.md).
# It's cheap enough to just run by hand (or have the agent run) before committing, per the
# session wrap-up checklist in AGENTS.md. Exits non-zero if something needs a look.
#
# Checks:
#   1. No dangling relative markdown links under BuildYourOwn/.
#   2. Every BOM table row in hardware/README.md has a non-empty Datasheet column (mechanical version of the
#      "every part is traceable" rule in core-beliefs.md).

set -uo pipefail

repo_root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
cd "$repo_root"

status=0

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
echo "== BOM datasheet links (BuildYourOwn/hardware/README.md) =="
bom_file="BuildYourOwn/hardware/README.md"
bom_issue=0
if [ -f "$bom_file" ]; then
  while IFS= read -r line; do
    case "$line" in
      '|'*) ;;
      *) continue ;;
    esac
    # Skip header row and separator row.
    case "$line" in
      *Part*Qty*|*'---'*) continue ;;
    esac
    IFS='|' read -r _ col_part _ _ col_datasheet _ <<< "$line"
    datasheet_trimmed="$(printf '%s' "${col_datasheet:-}" | sed -E 's/^[[:space:]]+//; s/[[:space:]]+$//')"
    if [ -z "$datasheet_trimmed" ]; then
      part_trimmed="$(printf '%s' "${col_part:-}" | sed -E 's/^[[:space:]]+//; s/[[:space:]]+$//')"
      echo "WARN: BOM row '$part_trimmed' has no datasheet link (core-beliefs.md: every part is traceable)."
      bom_issue=1
    fi
  done < "$bom_file"
fi
if [ "$bom_issue" -eq 1 ]; then
  status=1
else
  echo "OK (no rows, or all rows have a datasheet link)"
fi

echo
if [ "$status" -eq 0 ]; then
  echo "All checks passed."
else
  echo "Some checks need attention (see WARN/BROKEN above)."
fi
exit "$status"
