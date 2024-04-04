from dataclasses import dataclass

@dataclass
class Startswith:
    value: str
    start: str = ""

    def __eq__(self, __value: object) -> bool:
        try:
            if self.value.startswith(__value):
                self.start = __value
                return True
        except:
            pass
        return False

    def without_start(self) -> str:
        return self.value.removeprefix(self.start)
