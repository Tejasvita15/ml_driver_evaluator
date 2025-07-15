def check_debug_support(code):
    debug_indicators = ["printk", "pr_debug", "dev_dbg", "#ifdef DEBUG"]
    found = any(indicator in code for indicator in debug_indicators)
    return {"debug_support": 1.0 if found else 0.3}