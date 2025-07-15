import os
import json
import sys
sys.path.append(os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))
from evaluation_engine.compile_checker import compile_code
from evaluation_engine.static_analysis import static_analysis
from evaluation_engine.code_quality_scanner import code_quality_analysis
from evaluation_engine.performance_analysis import check_efficiency
from evaluation_engine.debug_support_analysis import check_debug_support
from evaluation_engine.scoring import calculate_score

CODE_DIR = "generated_code"
RESULTS_DIR = "results"
os.makedirs(RESULTS_DIR, exist_ok=True)

for filename in os.listdir(CODE_DIR):
    if filename.endswith(".c"):
        path = os.path.join(CODE_DIR, filename)
        with open(path) as f:
            code = f.read()

        compilation = compile_code(CODE_DIR)
        security = static_analysis(code)
        code_quality = code_quality_analysis(code)
        performance = check_efficiency(code)
        advanced = check_debug_support(code)

        metrics = {
            "compilation": compilation,
            "security": security,
            "code_quality": code_quality,
            "performance": performance,
            "advanced": advanced
        }

        metrics["overall_score"] = calculate_score(metrics)

        with open(os.path.join(RESULTS_DIR, filename.replace(".c", "_eval.json")), "w") as out:
            json.dump(metrics, out, indent=4)