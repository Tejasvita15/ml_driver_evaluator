import os
import json
from groq import Groq
from datetime import datetime
from dotenv import load_dotenv

# Load variables from .env file
load_dotenv()

# Now this will work:
client = Groq(api_key=os.getenv("GROQ_API_KEY"))

OUTPUT_DIR = "generated_code"
PROMPT_FILE = "prompts/sample_prompts.json"
os.makedirs(OUTPUT_DIR, exist_ok=True)

# Load prompts from file
with open(PROMPT_FILE, "r") as f:
    prompts = json.load(f)

# Prompt to guide the LLM
SYSTEM_PROMPT = (
    "You are an expert Linux kernel developer. Generate a complete Linux device driver in C "
    "based on the prompt provided. Use appropriate kernel APIs, follow Linux coding style, "
    "and include basic error handling. Output only the complete C code in a single file."
)

def generate_driver(prompt_text):
    chat_completion = client.chat.completions.create(
        messages=[
            {"role": "system", "content": SYSTEM_PROMPT},
            {"role": "user", "content": prompt_text}
        ],
        model="llama3-70b-8192",  # You can use other Groq-supported models too
        temperature=0.4,
        max_tokens=1400
    )
    return chat_completion.choices[0].message.content

# Generate code for each prompt
for idx, item in enumerate(prompts):
    prompt = item.get("prompt")
    print(f"🚀 Generating for Prompt {idx + 1}: {prompt}")

    try:
        generated_code = generate_driver(prompt)
        filename = f"driver_{idx+1}_{datetime.now().strftime('%Y%m%d%H%M%S')}.c"
        filepath = os.path.join(OUTPUT_DIR, filename)

        with open(filepath, "w") as f:
            f.write(generated_code)

        print(f"✅ Saved: {filepath}")

    except Exception as e:
        print(f"❌ Error generating code for Prompt {idx + 1}: {e}")

# Optional: Run evaluation after generation
os.system("python scripts/run_evaluation.py")
