Overview

This project is a Linux Device Driver Evaluation System powered by an LLM (Large Language Model) pipeline. It automatically generates .c driver files from natural language prompts and evaluates their quality based on compilation, security, performance, and other static code analysis metrics.

Flow: Prompt → LLM → .c File → Evaluation → Score

Prompt: Prompts are stored in prompts/sample_prompts.json. These describe what type of Linux driver to generate.

LLM Generation (Groq): The generate_drivers_groq.py script uses the Groq API (e.g., LLaMA3-70B) to generate C code from each prompt.

File Output: Each generated driver is saved in generated_code/ as a .c file.

Evaluation: The run_evaluation.py script runs static and rule-based evaluations on each .c file.

Scoring: Final results are saved as structured JSON files in the results/ folder, showing per-metric breakdowns and total score.

Tech Stack

LLM API: Groq API with llama3-70b-8192

Python 3.10+

Static Analysis Tools: Regex, logical rule matching, and scoring heuristics

Evaluation Metrics: Compilation checks, style compliance, buffer safety, etc.

Environment: .env file for API key management using python-dotenv

