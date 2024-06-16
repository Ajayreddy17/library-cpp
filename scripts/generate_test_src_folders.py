
import logging
import textwrap
import argparse
from pathlib import Path

def generate(relative_path: Path):
    
    dir_path = gen_root / relative_path

    if dir_path.exists():
        logger.debug(f'Directory "{dir_path}" already exists.')
        return

    logger.info(f'Generate directory "{dir_path}."')

    dir_path.mkdir(parents=True)
    gitignore = dir_path / ".gitignore"

    gitignore.write_text(
        textwrap.dedent("""\
            /*
            !.gitignore
            !*.cpp
            combined.cpp
            !*.md
        """)
    )

if __name__ == '__main__':
    logger: logging.Logger = logging.getLogger(__name__)

    src_root = Path(r"C:\Users\thati\Cp\library-cpp\library")
    gen_root = Path(r"C:\Users\thati\Cp\library-cpp\test")

    for src_path in src_root.glob('**/*.hpp'):
        generate(src_path.relative_to(src_root).with_suffix(''))