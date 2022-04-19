/*
BSD 3-Clause License

Copyright (c) 2022, ccbrantley
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this
   list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its
   contributors may be used to endorse or promote products derived from
   this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <chrono>
#include <iostream>
#include <random>
#include <unistd.h>
#include <vector>
#include <X11/Xlib.h>

class Grid
{

    private:

        int xUnits;
        int yUnits;
        std::vector<std::vector<int>> grid;

    public:

       Grid(int _xUnits, int _yUnits)
        {
            this->xUnits = _xUnits;
            this->yUnits = _yUnits;
        }

        // Mutators.

        void setXUnits(int _xUnits) { this->xUnits = _xUnits; }

        void setYUnits(int _yUnits) { this->yUnits = _yUnits; }

        // Accessors.

        int getXUnits() { return this->xUnits; }

        int getYUnits() { return this->yUnits; }

        std::vector<std::vector<int>> & getGrid() { return this->grid; }

        ~Grid()
        {
        }

};

class GridOfLife : public Grid
{
    private:

        std::vector<std::vector<int>> liveNeighbours;

    public:

        GridOfLife(int _xUnits, int _yUnits) :
        Grid(_xUnits, _yUnits)
        {
            this->initializeGrid();
            this->initializeLiveNeighbours();
        }

    // Mutators.

    // Accessors.

    private:

        void initializeGrid()
        {
            int xUnits = this->getXUnits();
            int yUnits = this->getYUnits();
            std::vector<std::vector<int>> & grid = this->getGrid();
            std::random_device randomDevice;
            std::mt19937 gen(randomDevice());
            std::uniform_int_distribution<> distr(0, 1);
            for (int x = 0; x < xUnits; x++)
            {
                grid.push_back({});
                for (int y = 0; y < yUnits; y++)
                {
                    if (!distr(gen))
                    {
                        grid[x].push_back(1);
                    }
                    else
                    {
                        grid[x].push_back(0);
                    }
                }
            }
        }

        void initializeLiveNeighbours()
        {
            for (int x = 0; x < this->getXUnits(); x++)
            {
                this->liveNeighbours.push_back({});
                for (int y = 0; y < this->getYUnits(); y++)
                {
                    this->liveNeighbours[x].push_back(0);
                }
            }
        }

    public:

        void calculateLiveNeighbours()
        {
            int xUnits = this->getXUnits();
            int yUnits = this->getYUnits();
            std::vector<std::vector<int>> & grid = this->getGrid();
            int liveNeighbours = 0;

            // Calculate top left unit live neighbours.

            liveNeighbours += grid[0][1];
            liveNeighbours += grid[1][1];
            liveNeighbours += grid[1][0];
            this->liveNeighbours[0][0] = liveNeighbours;
            liveNeighbours = 0;

            // Calculate bottom left unit live neighbours.

            liveNeighbours += grid[xUnits - 2][0];
            liveNeighbours += grid[xUnits - 2][1];
            liveNeighbours += grid[xUnits - 1][1];
            this->liveNeighbours[xUnits - 1][0] = liveNeighbours;
            liveNeighbours = 0;

            // Calculate top right unit live neighbours.

            liveNeighbours += grid[1][yUnits - 1];
            liveNeighbours += grid[1][yUnits - 2];
            liveNeighbours += grid[0][yUnits - 2];
            this->liveNeighbours[0][yUnits - 1] = liveNeighbours;
            liveNeighbours = 0;

            // Calculate bottom right unit live neighbours.

            liveNeighbours += grid[xUnits - 2][yUnits - 1];
            liveNeighbours += grid[xUnits - 1][yUnits - 2];
            liveNeighbours += grid[xUnits - 2][yUnits - 2];
            this->liveNeighbours[xUnits - 1][yUnits - 1] = liveNeighbours;
            liveNeighbours = 0;

            // Calculate the leftmost middle live neighbours.

            for (int x = 1; x < xUnits - 1; x++)
            {
                liveNeighbours += grid[x - 1][0];
                liveNeighbours += grid[x - 1][1];
                liveNeighbours += grid[x][1];
                liveNeighbours += grid[x + 1][1];
                liveNeighbours += grid[x + 1][0];
                this->liveNeighbours[x][0] = liveNeighbours;
                liveNeighbours = 0;
            }

            // Calculate the rightmost middle live neighbours.

            for (int x = 1; x < xUnits - 1; x++)
            {
                liveNeighbours += grid[x - 1][yUnits - 1];
                liveNeighbours += grid[x + 1][yUnits - 1];
                liveNeighbours += grid[x + 1][yUnits - 2];
                liveNeighbours += grid[x][yUnits - 2];
                liveNeighbours += grid[x - 1][yUnits - 2];
                this->liveNeighbours[x][yUnits - 1] = liveNeighbours;
                liveNeighbours = 0;
            }

            // Calculate  the topmost middle live neighbours.

            for (int y = 1; y < yUnits - 1; y++)
            {
                liveNeighbours += grid[0][y + 1];
                liveNeighbours += grid[1][y + 1];
                liveNeighbours += grid[1][y];
                liveNeighbours += grid[1][y - 1];
                liveNeighbours += grid[0][y - 1];
                this->liveNeighbours[0][y] = liveNeighbours;
                liveNeighbours = 0;
            }

            // Calculate the bottommost middle live neighbours.

            for (int y = 1; y < yUnits - 1; y++)
            {
                liveNeighbours += grid[xUnits - 2][y];
                liveNeighbours += grid[xUnits - 2][y + 1];
                liveNeighbours += grid[xUnits - 1][y + 1];
                liveNeighbours += grid[xUnits - 1][y - 1];
                liveNeighbours += grid[xUnits - 2][y - 1];
                this->liveNeighbours[xUnits - 1][y] = liveNeighbours;
                liveNeighbours = 0;
            }

            // Calculate the middlemost live neighbours.

            for (int x = 1; x < xUnits - 1; x++)
            {
                for (int y = 1; y < yUnits - 1; y++)
                {
                    liveNeighbours += grid[x - 1][y];
                    liveNeighbours += grid[x - 1][y + 1];
                    liveNeighbours += grid[x][y + 1];
                    liveNeighbours += grid[x + 1][y + 1];
                    liveNeighbours += grid[x + 1][y];
                    liveNeighbours += grid[x + 1][y - 1];
                    liveNeighbours += grid[x][y - 1];
                    liveNeighbours += grid[x - 1][y - 1];
                    this->liveNeighbours[x][y] = liveNeighbours;
                    liveNeighbours = 0;
                }
            }
        }

        void applyRules()
        {
            int yUnits = this->getYUnits();
            std::vector<std::vector<int>> & grid = this->getGrid();
            int liveNeighbours = 0;
            for (int x = 0; x < this->getXUnits(); x++)
            {
                for (int y = 0; y < yUnits; y++)
                {
                    liveNeighbours = this->liveNeighbours[x][y];
                    if (liveNeighbours == 3)
                    {
                        grid[x][y] = 1;
                    }
                    else if ((liveNeighbours == 2) && grid[x][y])
                    {
                        grid[x][y] = 1;
                    }
                    else
                    {
                        grid[x][y] = 0;
                    }
                }
            }
        }

        void nextGeneration()
        {
            this->calculateLiveNeighbours();
            this->applyRules();
        }

};

class ConwayDisplay
{

    private:

        int width;
        int height;
        int colorBlack;
        int colorWhite;
        Display *display;
        Window window;
        XColor backgroundColor;
        GC graphicsContext;
        XEvent event;
        std::chrono::high_resolution_clock::time_point timePoint;
        int fps;

    public:

        ConwayDisplay(int _width, int _height, int _fps)
        {
            this->width = _width;
            this->height = _height;
			this->fps = _fps;
            this->display = XOpenDisplay(NULL);
            if (this->display == NULL)
            {
                exit(1);
            }
            this->colorBlack = BlackPixel(this->display, DefaultScreen(this->display));
            this->colorWhite = WhitePixel(this->display, DefaultScreen(this->display));
            this->window = XCreateSimpleWindow(
                this->display,
                XDefaultRootWindow(this->display),
                0,
                0,
                this->width,
                this->height,
                0,
                this->colorBlack,
                this->colorWhite
                );
            XMapWindow(
                this->display,
                this->window
                );
            this->graphicsContext = XCreateGC(
                this->display,
                this->window,
                0,
                0
                );
            XSelectInput(
                this->display,
                this->window,
                ExposureMask |
                KeyPressMask
                );
        }

        // Mutators.

        // Accessors.

        Display * getDisplay() { return this->display; }

        XEvent * getEvent() { return &this->event; }

        GC getGraphicsContext() { return this->graphicsContext; }

        Window getWindow() { return this->window; }

        int nextEvent()
        {
            XNextEvent(
                this->display,
                &this->event
                );
            if (this->event.type == Expose)
            {
                this->clearWindow();
                return Expose;
            }
            return -1;
        }

        void clearWindow()
        {
            XClearWindow(
                this->display,
                this->window
                );
        }

        void throwEventExpose()
        {
	        XExposeEvent exposeEvent;
	        exposeEvent.type = Expose;
            XSendEvent(
                this->display,
                this->window,
                false,
                ExposureMask,
                (XEvent *) &exposeEvent
                );
        }

        void drawPoint(int _x, int _y)
        {
            XDrawPoint(
                this->display,
                this->window,
                this->graphicsContext,
                _x,
                _y
                );
        }

        void fillRectangle(int _x, int _y, int _width, int _height)
        {
            XFillRectangle(
                this->display,
                this->window,
                this->graphicsContext,
                _x,
                _y,
                _width,
                _height
                );
        }

        void tick()
        {
            this->timePoint = std::chrono::high_resolution_clock::now();
        }

        void synchTicks()
        {
	        std::chrono::duration<double, std::milli> milliseconds = std::chrono::high_resolution_clock::now() - this->timePoint;
            while (milliseconds.count() < (100 / this->fps))
            {
                milliseconds = std::chrono::high_resolution_clock::now() - this->timePoint;
            }
        }

        ~ConwayDisplay()
        {
            XFreeGC(
                this->display,
                this->graphicsContext
                );
            XUnmapWindow(
                this->display,
                this->window
                );
            XCloseDisplay(
                this->display
            );
        }

};

int main ()
{
    int xUnits = 80;
    int yUnits = 80;
    int width = 800;
    width -= width % xUnits;
    int height = 800;
    height -= height % yUnits;
    GridOfLife gridOfLife(xUnits, yUnits);
    ConwayDisplay conwayDisplay(width, height, 15);
    while (true)
    {
        conwayDisplay.tick();
        int eventType = conwayDisplay.nextEvent();
        if (eventType == Expose)
        {
            std::vector<std::vector<int>> & grid = gridOfLife.getGrid();
            for (int x = 0; x < xUnits; x++)
            {
                for (int y = 0; y < yUnits; y++)
                {
                    if (grid[x][y])
                    {
                        if ((xUnits == width) && (yUnits == height))
                        {
                            conwayDisplay.drawPoint(x, y);
                        }
                        else
                        {
                            conwayDisplay.fillRectangle(
                                x * (width / xUnits),
                                y * (height / yUnits),
                                width / xUnits,
                                height / yUnits
                                );
                        }
                    }
                }
            }
        }
        gridOfLife.nextGeneration();
        conwayDisplay.throwEventExpose();
        conwayDisplay.synchTicks();
    }
    return 0;
}
