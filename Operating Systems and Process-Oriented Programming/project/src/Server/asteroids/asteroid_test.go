package asteroids

import "testing"

//Movement a asteroid three times in a pase of xStep and yStep
func Movement(xStep int, yStep int) *Asteroid {

	a := newAsteroid()
	a.X = 5
	a.Y = 5

	a.xStep = xStep
	a.yStep = yStep

	a.move()
	a.move()
	a.move()

	return a
}

func CreateFakeAsteroid(x int, y int) *Asteroid {

	a := newAsteroid()
	a.X = x
	a.Y = y

	return a
}

// TestMovementHorisontal tests to move a asteroid Horisontal
func TestMovementHorisontal(t *testing.T) {

	var roid1 = Movement(1, 0)

	if roid1.X != 8 || roid1.Y != 5 {
		t.Error("Expected position (x,y) = (8,5), got (", roid1.X, ",", roid1.Y, ")")
	}

	var roid2 = Movement(-1, 0)

	if roid2.X != 2 || roid2.Y != 5 {
		t.Error("Expected position (x,y) = (2,5), got (", roid2.X, ",", roid2.Y, ")")
	}
}

// TestMovementVertical test to move a asteroid vertical

func TestMovementVertical(t *testing.T) {
	var roid1 = Movement(0, 1)

	if roid1.X != 5 || roid1.Y != 8 {
		t.Error("Expected position (x,y) = (5,8), got (", roid1.X, ",", roid1.Y, ")")
	}

	var roid2 = Movement(0, -1)

	if roid2.X != 5 || roid2.Y != 2 {
		t.Error("Expected position (x,y) = (5,2), got (", roid2.X, ",", roid2.Y, ")")
	}
}

// TestMovementDiagonal test to move a asteroid in a diagonal
func TestMovementDiagonal(t *testing.T) {
	var roid1 = Movement(1, 1)

	if roid1.X != 8 || roid1.Y != 8 {
		t.Error("Expected position (x,y) = (8,8), got (", roid1.X, ",", roid1.Y, ")")
	}

	var roid2 = Movement(-1, -1)

	if roid2.X != 2 || roid2.Y != 2 {
		t.Error("Expected position (x,y) = (2,2), got (", roid2.X, ",", roid2.Y, ")")
	}
}

//TestInBounds tests if an asteroid is inside the gameboard
func TestInBounds(t *testing.T) {

	var manager = newAsteroidManager()
	manager.xMax = 100
	manager.yMax = 100

	var a1 = CreateFakeAsteroid(-1, (manager.yMax + 1))
	var a2 = CreateFakeAsteroid(-1, (manager.yMax - 1))
	var a3 = CreateFakeAsteroid(10, (manager.yMax + 100))

	if a1.inBounds(manager) {
		t.Error("Error in X or Y-coordinate when Y > yMax and X < 0 ")
	}

	if a2.inBounds(manager) {
		t.Error("Error in X when X < 0  ")
	}

	if a3.inBounds(manager) {
		t.Error("Error in Y when Y > yMax ")
	}

	for i := 0; i < manager.xMax; i++ {

		var a = CreateFakeAsteroid(i, 0)

		if !a.inBounds(manager) {
			t.Error("Error when asteroid is inbound on coordinate: (", a.X, ",", a.Y, ")")
		}
	}

	for i := 0; i < manager.yMax; i++ {

		var a = CreateFakeAsteroid(0, i)

		if !a.inBounds(manager) {
			t.Error("Error when asteroid is inbound on coordinate: (", a.X, ",", a.Y, ")")
		}
	}

}
