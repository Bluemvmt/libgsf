# libgsf (GSF 3.11)

Linux shared-library build of the [Generic Sensor Format](https://www.leidos.com/products/ocean-marine) library, customized for the [bluemvmt-gsf](https://github.com/Bluemvmt/bluemvmt-gsf) Python package.

This repository supports **GSF 3.11 only**. Upstream 3.11 is a maintenance release over 3.10 (unsigned `beam_angle_forward` codec restored; `start_range_samples` copied in `gsfCopyRecords`). Local JSON helpers (`gsfOpenForJson`, `gsfNextJsonRecord`, etc.) are ported on top of that tree so the Python `ctypes` bindings keep working. Older on-disk GSF files remain readable through the upstream compatibility paths.

## Supported platforms

| Host | Target artifact |
|------|-----------------|
| Linux (native) | `dist/libgsf-$(uname -m)-03.11.so` |
| Windows / macOS / Linux via Docker Buildx | `libgsf-x86_64-03.11.so` and `libgsf-aarch64-03.11.so` |

Only **Linux** shared objects are produced. Native Windows/macOS binaries are out of scope; use the upstream Leidos distribution if you need those.

Artifact names match the consumer contract:

```text
src/bluemvmt_gsf/libgsf/lib/libgsf-{machine()}-03.11.so
```

## Native Linux build

Requires `gcc`, GNU Make, `binutils` (`nm`, `readelf`), and `file`.

The Makefile pins `-std=gnu11`. Do not drop that flag: GCC 14+ defaults to
C23 and will emit `__isoc23_*` imports that need `GLIBC_2.38`, which breaks
Debian bookworm / Ubuntu 22.04 consumers (for example `python:*-bookworm`).
Prefer the Docker Buildx path below when possible.

```sh
make          # builds dist/libgsf-$(uname -m)-03.11.so
make smoke    # verify ELF machine type, exports, and glibc max version
make clean
```

## Docker Buildx (Windows / macOS / Linux)

Produces both architectures in one command. Multi-platform output requires a Buildx builder using the `docker-container` driver (Docker Desktop includes this; plain Docker Engine defaults to the single-platform `docker` driver).

One-time builder setup:

```sh
docker buildx create --name libgsf-multi --driver docker-container --use
docker buildx inspect --bootstrap
```

On Linux hosts without Docker Desktop, also register QEMU user-mode emulation once:

```sh
docker run --privileged --rm tonistiigi/binfmt --install all
```

Then build both architectures:

```sh
docker buildx build \
  --platform linux/amd64,linux/arm64 \
  --output type=local,dest=dist \
  .
```

Output layout (multi-platform):

```text
dist/
  linux_amd64/libgsf-x86_64-03.11.so
  linux_arm64/libgsf-aarch64-03.11.so
```

Copy those files into `bluemvmt-gsf` at `src/bluemvmt_gsf/libgsf/lib/`.

Single-architecture example (works with the default Docker driver):

```sh
docker buildx build \
  --platform linux/amd64 \
  --output type=local,dest=dist \
  .
```

That writes `dist/libgsf-x86_64-03.11.so` directly.

## Downstream follow-up (bluemvmt-gsf)

Updating the Python package is a separate change:

1. Add `_3_11 = "03.11"` to `GsfVersion` and default to it.
2. Vendor both new `.so` files under `src/bluemvmt_gsf/libgsf/lib/`.
3. Update the `gsfOpenForJson` ctypes call for the sixth `flattened_version` argument introduced by [libgsf PR #8](https://github.com/Bluemvmt/libgsf/pull/8).

## Layout

```text
GSF_03-11/     Upstream 3.11 sources + local gsf_json.c/h + official PDFs
cjson/         Bundled cJSON dependency used by the JSON helpers
scripts/       Smoke-check helper used by `make smoke` and the Docker build
Dockerfile     Multi-arch Linux builder
Makefile       Single shared-object build
```

## Provenance

Upstream source and documentation: [Leidos Ocean & Marine products](https://www.leidos.com/products/ocean-marine). See `GSF_03-11/gsf_version_03_11_change_summary.pdf` for the official 3.11 change requests.
