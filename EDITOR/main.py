from settings import *
from grid import Grid


class Editor :
    def __init__(self) :
        self._screen = pg.display.set_mode((0,0), pg.FULLSCREEN | pg.NOFRAME)
        pg.event.set_grab(True)
        pg.mouse.set_visible(False)
        
        self.grid = Grid(self, [100,100, 40])
        self.end_pos = False
        
        self.start, self.end = (0,0), (0,0)
        
    def render(self) :
        self._screen.fill(0)
        
        self.grid.render()
        
        pg.draw.circle(self._screen, (255,255,255), self.grid.get_mouse_pos(), 6, 2)
        
        if pg.mouse.get_pressed()[1] :
            self.grid.set_offset(pg.mouse.get_rel())
        
        
        
        
    def update(self) :
        pg.display.flip()
        
        
    def events(self) :
        for e in pg.event.get() :
            if e.type == pg.QUIT or e.type == pg.KEYDOWN and e.key == pg.K_ESCAPE :
                quit()
            # if e.type == pg.MOUSEWHEEL :
            #     self.grid.zoom(e.y)
            # capture right-click position only on button down
            pos = self.get_right_click_pos_on_down(e)
            if pos and not self.end_pos:
                # example action: print or store position
                self.start = pos
                self.end_pos = True
            elif pos and self.end_pos:
                self.end = pos
                self.grid.place_wall(self.start - self.grid._offset, self.end - self.grid._offset)
                self.end_pos = False
                



    def get_right_click_pos_on_down(self, event):
        """Return the grid-aligned mouse position when the right mouse
        button is pressed down. If the event is not a right-button
        MOUSEBUTTONDOWN, returns None.
        """
        if event.type == pg.MOUSEBUTTONDOWN and getattr(event, 'button', None) == 3:
            return self.grid.get_mouse_pos()
        return None
        
        
        
        
    def run(self) :
        while True :
            self.render()
            self.update()
            self.events()
            


if __name__ == "__main__" :
    app = Editor()
    app.run()