
import os
import subprocess

project_dir = os.path.dirname(os.path.abspath(__file__))
build_dir = os.path.join(project_dir, "build")
exe_path = os.path.join(build_dir, "Debug", "GndG3.0.exe")

# CMake configure
subprocess.run(["cmake", "-S", ".", "-B", "build"], cwd=project_dir)

# CMake build
build_result = subprocess.run(["cmake", "--build", "build", "--config", "Debug"], cwd=project_dir)
if build_result.returncode != 0:
    print("❌ Build failed.")
    exit(build_result.returncode)

# Run the executable
if os.path.exists(exe_path):
    print("🚀 Running executable...")
    subprocess.run([exe_path])
else:
    print(f"❌ Executable not found at {exe_path}")
