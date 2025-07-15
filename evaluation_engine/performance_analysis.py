def check_efficiency(code):
    line_count = len(code.strip().splitlines())
    efficiency = 1.0 if line_count <= 150 else 0.7 if line_count <= 250 else 0.5
    return {"efficiency": efficiency}