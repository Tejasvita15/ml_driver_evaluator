import re

def static_analysis(code):
    buffer_safety = 1.0 if "copy_to_user" in code or "copy_from_user" in code else 0.0
    race_conditions = 1.0 if "mutex" in code or "spinlock" in code else 0.5
    input_validation = 1.0 if "if" in code and "return -EINVAL" in code else 0.5
    return {
        "buffer_safety": buffer_safety,
        "race_conditions": race_conditions,
        "input_validation": input_validation
    }