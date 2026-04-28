from flask import Flask, request, jsonify, send_from_directory
import subprocess
import os
import re

# ===============================
#  PATH SETUP
# ===============================
BASE_DIR = os.path.dirname(os.path.abspath(__file__))
PROJECT_ROOT = os.path.abspath(os.path.join(BASE_DIR, '..'))

UI_DIR = os.path.join(PROJECT_ROOT, 'ui')
INPUT_DIR = os.path.join(PROJECT_ROOT, 'input')
EXE_PATH = os.path.join(PROJECT_ROOT, 'main.exe')

app = Flask(__name__, static_folder=UI_DIR, static_url_path='')


# ===============================
# ✅ SERVE UI
# ===============================
@app.route('/')
def index():
    return send_from_directory(UI_DIR, 'index.html')


# ===============================
#  RUN COMPILER
# ===============================
@app.route('/run', methods=['POST'])
def run():
    try:
        print("\n🔥 RUN API CALLED")

        # ---------------------------
        # Validate file
        # ---------------------------
        if 'file' not in request.files:
            return jsonify({"error": "No file uploaded"}), 400

        file = request.files['file']

        # ---------------------------
        # Save file
        # ---------------------------
        os.makedirs(INPUT_DIR, exist_ok=True)
        input_path = os.path.join(INPUT_DIR, file.filename)
        file.save(input_path)

        print("📂 Input:", input_path)

        # ---------------------------
        # Check EXE
        # ---------------------------
        if not os.path.exists(EXE_PATH):
            return jsonify({"error": "main.exe not found"}), 500

        print("⚙ Running compiler...")

        # ---------------------------
        # Run C++ program
        # ---------------------------
        result = subprocess.run(
            [EXE_PATH, input_path],
            capture_output=True,
            text=True,
            cwd=PROJECT_ROOT
        )

        full_output = result.stdout + "\n" + result.stderr

        print("==== OUTPUT ====")
        print(full_output)

        # ===============================
        # CLEAN SPILL COST PARSING
        # ===============================
        spill_dict = {}

        pattern = r"\[SPILL\]\s+(\w+)\s+cost=([0-9\.]+)"

        for line in full_output.split("\n"):
            match = re.search(pattern, line.strip())
            if match:
                var = match.group(1)
                cost = float(match.group(2))

                if var not in spill_dict or cost > spill_dict[var]:
                    spill_dict[var] = cost

        # ---------------------------
        # Sort spills (HIGH → LOW)
        # ---------------------------
        sorted_spills = sorted(
            [{"var": k, "cost": v} for k, v in spill_dict.items()],
            key=lambda x: x["cost"],
            reverse=True
        )

        print("🔥 Final Spill Costs:", sorted_spills)

        # ===============================
        #  FIX: SIMPLE SPILL LIST
        # ===============================
        spill_list = list(spill_dict.keys())

        # ===============================
        # GRAPH GENERATION
        # ===============================
        dot_path = "dot"

        cfg_png = os.path.join(UI_DIR, 'cfg.png')
        embedded_png = os.path.join(UI_DIR, 'embedded.png')
        desktop_png = os.path.join(UI_DIR, 'desktop.png')

        # Remove old images
        for f in [cfg_png, embedded_png, desktop_png]:
            if os.path.exists(f):
                os.remove(f)

        # Generate graphs
        subprocess.run([dot_path, "-Tpng", "cfg.dot", "-o", cfg_png], cwd=PROJECT_ROOT)
        subprocess.run([dot_path, "-Tpng", "Embedded_interference.dot", "-o", embedded_png], cwd=PROJECT_ROOT)
        subprocess.run([dot_path, "-Tpng", "Desktop_interference.dot", "-o", desktop_png], cwd=PROJECT_ROOT)

        print("✅ Graphs generated")

        # ===============================
        #  RESPONSE
        # ===============================
        return jsonify({
            "cfg": "cfg.png",
            "embedded": "embedded.png",
            "desktop": "desktop.png",
            "output": full_output,
            "spill_costs": sorted_spills,  # clean structured data
            "spills": spill_list           # 🔥 FIXED (for Spill Analysis)
        })

    except Exception as e:
        print("❌ ERROR:", str(e))
        return jsonify({"error": str(e)}), 500


# ===============================
# DISABLE CACHE
# ===============================
@app.after_request
def add_header(response):
    response.headers["Cache-Control"] = "no-cache, no-store, must-revalidate"
    response.headers["Pragma"] = "no-cache"
    response.headers["Expires"] = "0"
    return response


# ===============================
#  RUN SERVER
# ===============================
if __name__ == '__main__':
    app.run(debug=True)