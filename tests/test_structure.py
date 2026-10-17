"""Small automated regression checks for the source tree."""
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]


class StructureTests(unittest.TestCase):
    def test_no_source_includes(self):
        header = (ROOT / "game.h").read_text(encoding="utf-8")
        self.assertNotIn('#include "screens/game.c"', header)
        self.assertNotIn('#include "entities/enemy.c"', header)
        for file in ROOT.rglob("*.c"):
            self.assertNotIn('#include "mecanics/', file.read_text(encoding="utf-8"))

    def test_single_power_up_implementation(self):
        self.assertTrue((ROOT / "entities/power_up.c").is_file())
        self.assertFalse((ROOT / "mecanics/power_up.c").exists())

    def test_safe_projectile_targets(self):
        src = (ROOT / "entities/projectile.c").read_text(encoding="utf-8")
        self.assertIn("target_id", src)
        self.assertIn("if (index < 0)", src)

    def test_external_assets_are_present(self):
        for filename in ["COUR.TTF", "sprites/nave.png", "txt_files/shortwords.txt"]:
            with self.subTest(filename=filename):
                self.assertTrue((ROOT / filename).is_file())


if __name__ == "__main__":
    unittest.main()
