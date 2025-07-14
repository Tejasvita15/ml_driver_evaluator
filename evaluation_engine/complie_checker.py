import subprocess
import os

def compile_code(code_path):
    makefile = os.path.join(code_path, "Makefile")
    if not os.path.exists(makefile):
        return {"success_rate": 0.0, "warnings_count": 0, "errors_count": 1, "log": "Makefile not found."}
    result = subprocess.run(["make"], cwd=code_path, capture_output=True, text=True)
    success = result.returncode == 0
    return {
        "success_rate": 1.0 if success else 0.0,
        "warnings_count": result.stderr.count("warning"),
        "errors_count": result.stderr.count("error"),
        "log": result.stderr + result.stdout
    }