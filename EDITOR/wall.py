from settings import *



class Wall :
    def __init__(self, start: glm.vec2, end: glm.vec2) :
        self._start: glm.vec2 = start
        self._end: glm.vec2 = end