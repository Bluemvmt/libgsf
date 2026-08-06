#!/usr/bin/env bash
# Verify a built libgsf shared object: ELF class/machine and exported symbols.
set -euo pipefail

SO="${1:?usage: smoke-check.sh <path-to-libgsf.so>}"

if [[ ! -f "$SO" ]]; then
  echo "error: shared object not found: $SO" >&2
  exit 1
fi

echo "==> file"
file "$SO"

echo "==> ELF header"
readelf -h "$SO" | awk '/Class:|Machine:|Type:/'

# Map expected uname -m / filename arch to ELF Machine field.
basename_so="$(basename "$SO")"
case "$basename_so" in
  libgsf-x86_64-*.so)
    expected_machine="Advanced Micro Devices X86-64"
    ;;
  libgsf-aarch64-*.so)
    expected_machine="AArch64"
    ;;
  *)
    echo "error: unexpected library name (expected libgsf-x86_64-*.so or libgsf-aarch64-*.so): $basename_so" >&2
    exit 1
    ;;
esac

machine="$(readelf -h "$SO" | awk -F: '/Machine:/ {sub(/^[[:space:]]+/, "", $2); print $2}')"
if [[ "$machine" != "$expected_machine" ]]; then
  echo "error: ELF Machine='$machine', expected '$expected_machine' for $basename_so" >&2
  exit 1
fi

echo "==> required exports"
required=(
  gsfOpenForJson
  gsfCloseForJson
  gsfNextJsonRecord
  gsfRecord_toJson
  gsfOpen
  gsfOpenBuffered
  gsfRead
  gsfSeek
  gsfClose
  gsfGetNumberRecords
  gsfIntError
  gsfStringError
)

exports="$(nm -D --defined-only "$SO" | awk '{print $3}')"
missing=0
for sym in "${required[@]}"; do
  if ! grep -qx "$sym" <<<"$exports"; then
    echo "  MISSING: $sym" >&2
    missing=1
  else
    echo "  ok: $sym"
  fi
done

if [[ "$missing" -ne 0 ]]; then
  echo "error: one or more required symbols are missing" >&2
  exit 1
fi

# Keep the ABI portable to Debian bookworm / Ubuntu 22.04 (glibc 2.35–2.36).
# Building with GCC 14+ in C23 mode pulls __isoc23_* @ GLIBC_2.38.
echo "==> glibc symbol versions"
glibc_versions="$(objdump -T "$SO" | grep -oE 'GLIBC_[0-9.]+' | sort -uV)"
echo "$glibc_versions"
if grep -qx 'GLIBC_2.38' <<<"$glibc_versions"; then
  echo "error: $SO requires GLIBC_2.38 (too new for bookworm/Ubuntu 22.04)." >&2
  echo "Rebuild with -std=gnu11 (see Makefile) or via the gcc:12-bookworm Docker image." >&2
  exit 1
fi

echo "smoke check passed: $SO"
