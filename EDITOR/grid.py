from settings import *
from wall import Wall



class Grid :
    def __init__(self,app,attr) :
        self.screen = app._screen
        self._atr = attr
        
        self._offset = pg.Vector2(0,0)
        
        self._walls: list[Wall] = []
        self._portals: list[Wall] = []
    
    
    def set_offset(self, offset) :
        self._offset += pg.Vector2(offset[0],offset[1])
    
    def zoom(self, amount) :
        self._atr[2] += amount * 2
        if self._atr[2] < 4 :
            self._atr[2] = 4
            
    def get_mouse_pos(self) :
        mx, my = pg.mouse.get_pos()
        grid_x = (mx - self._offset.x) // self._atr[2]
        grid_y = (my - self._offset.y) // self._atr[2]
        return (int(grid_x) * self._atr[2] + self._offset.x, int(grid_y) * self._atr[2] + self._offset.y)
    
    
    def place_wall(self, start, end) :
        self._walls.append(Wall(start, end))
        
    def place_portal(self, start, end) :
        self._portals.append(Wall(start, end))
    
    
    
    def render(self) :
        for x in range(self._atr[0]):
            for y in range(self._atr[1]):
                pg.draw.circle(self.screen, (100,100,100), ((x * self._atr[2] + self._offset.x),(y * self._atr[2]) + self._offset.y), 5)
    
        
        
        
        
        for wall in self._walls :
            pg.draw.line(self.screen, (250,100,100), (wall._start + self._offset), wall._end + self._offset, 2)
            pg.draw.circle(self.screen, (100,100,250), wall._start + self._offset, 6)
            pg.draw.circle(self.screen, (100,100,250), wall._end + self._offset, 6)
    
        for portal in self._portals :
            pg.draw.line(self.screen, (100,250,100), portal._start + self._offset, portal._end + self._offset, 2)