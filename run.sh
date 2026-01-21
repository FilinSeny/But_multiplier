#!/usr/bin/env bash
set -euo pipefail

GEN_BIN="./build/but_gen"
RESULTS_DIR="results"
IVERILOG_FLAGS="-g2012"

usage() {
  cat <<'EOF'
Usage:
  ./run_sim.sh --syn -- <but_gen flags...>

    Runs generator (./build/but_gen) with given flags.
    IMPORTANT: pass generator flags after "--".

    Example:
      ./run_sim.sh --syn -- --m 2 --r 2 --test 123 --path results/But_multiplier2x2

  ./run_sim.sh --run_tests <ModuleName>

    Compiles and runs testbench for the given module name.
    Expects files in:
      results/<ModuleName>/<ModuleName>.v
      results/<ModuleName>/<ModuleName>_testbanch.v

    Example:
      ./run_sim.sh --run_tests But_multiplier2x2

EOF
}

die() {
  echo "[ERROR] $*" >&2
  exit 1
}

need_file() {
  local f="$1"
  [ -f "$f" ] || die "file not found: $f"
}

if [ $# -lt 1 ]; then
  usage
  exit 1
fi

MODE="$1"
shift

case "$MODE" in
  --syn)
    # Require "--" separator to avoid ambiguity
    if [ $# -lt 1 ] || [ "${1:-}" != "--" ]; then
      die "expected '--' after --syn. Example: ./run_sim.sh --syn -- --m 2 --r 2 --path results"
    fi
    shift

    [ -x "$GEN_BIN" ] || die "generator not found/executable: $GEN_BIN (build it with cmake --build build)"

    echo "[INFO] Running generator: $GEN_BIN $*"
    "$GEN_BIN" "$@"
    ;;

  --run_tests)
    if [ $# -ne 1 ]; then
      die "usage: ./run_sim.sh --run_tests <ModuleName>"
    fi

    MODULE_NAME="$1"
    OUT_DIR="${RESULTS_DIR}"
    RTL_FILE="${OUT_DIR}/${MODULE_NAME}.v"
    TB_FILE="${OUT_DIR}/${MODULE_NAME}_testbanch.v"
    SIM_BIN="${OUT_DIR}/sim.out"

    echo "[INFO] Module: ${MODULE_NAME}"
    echo "[INFO] Output dir: ${OUT_DIR}"

    need_file "$RTL_FILE"
    need_file "$TB_FILE"

    echo "[INFO] Compiling with iverilog..."
    iverilog ${IVERILOG_FLAGS} -o "${SIM_BIN}" "${RTL_FILE}" "${TB_FILE}"

    echo "[INFO] Running simulation..."
    ( cd "${OUT_DIR}" && vvp "./$(basename "${SIM_BIN}")" )

    echo "[INFO] Done"
    ;;

  -h|--help)
    usage
    ;;

  *)
    die "unknown mode: $MODE (use --help)"
    ;;
esac
