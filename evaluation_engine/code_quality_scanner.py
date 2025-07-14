def code_quality_analysis(code):
    style_compliance = 1.0 if len(code) < 5000 else 0.7
    documentation = 0.5 if "/*" in code else 0.2
    maintainability = 0.8 if code.count("goto") < 3 else 0.5
    return {
        "style_compliance": style_compliance,
        "documentation": documentation,
        "maintainability": maintainability
    }