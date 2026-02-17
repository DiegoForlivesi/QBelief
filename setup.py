from setuptools import setup, Extension, find_packages
import pybind11
 
ext_modules = [
    Extension(
        'src.Simulation',                    # Module name
        ['src/main.cpp', 'src/DecodersBP.cpp'],              # C++ source file
        include_dirs=[pybind11.get_include()],
        extra_compile_args=["-std=c++17"],
        language='c++'
    ),
]
 
setup(
    name="DecoderBP",                            # match pyproject.toml
    version="1.0.0",
    packages=["src"],                            # 'src' is a real package; no package_dir
    ext_modules=ext_modules,
)