def calculate_score(metrics):
    score = (
        0.4 * metrics['compilation']['success_rate'] +
        0.25 * metrics['security']['buffer_safety'] +
        0.2 * metrics['code_quality']['style_compliance'] +
        0.1 * metrics['performance']['efficiency'] +
        0.05 * metrics['advanced']['debug_support']
    ) * 100
    return round(score, 2)
