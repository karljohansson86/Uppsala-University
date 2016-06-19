package asteroids

import "fmt"

// collisionManager used to get all collision that have occured during the last tick
func (world *World) collisionManager() {
	mutex.Lock()
	// removes old collisions
	world.removeCollisions()
	// First check player vs player and asteroid
	world.playerCollision()
	// Second check asteroid vs asteroid
	world.asteroidCollision()
	world.print()
	mutex.Unlock()
}

// asteroidCollision is used to check if two asteroids have collided
func (world *World) asteroidCollision() {

	for _, a1 := range world.Asteroids {
		//a1.mutex.Lock()
		for _, a2 := range world.Asteroids {
			// Check if same asteroid
			if a1.ID == a2.ID {
				continue
			}

			//a2.mutex.Lock()
			if isCollisionSize(a1.X, a1.Y, a1.size, a2.X, a2.Y, a2.size) {
				world.appendCollision(a1.X, a1.Y)
				a1.Alive = false
			}
			//a2.mutex.Unlock()
		}
		//a1.mutex.Unlock()
	}
}

// playerCollision is used to check if a player has collided with another player or asteroid
func (world *World) playerCollision() {

	for _, p := range world.Players {
		for _, a := range world.Asteroids {
			if isCollisionSize(p.X, p.Y, p.size, a.X, a.Y, a.size) {
				world.appendCollision(p.X, p.Y)
				p.Alive = false
				a.Alive = false
			}
		}

		for _, p2 := range world.Players {

			if isCollisionSize(p.X, p.Y, p.size, p2.X, p2.Y, p2.size) && p.ID != p2.ID {
				world.appendCollision(p.X, p.Y)
				p.Alive = false
			}

		}
	}

}

// TODO is this function in use anymore?
// isCollision checks if two objects are located at the same position
func isCollision(x1 int, y1 int, x2 int, y2 int) bool {
	return x1 == x2 && y1 == y2
}

// Abs returns the absolute value of x.
func Abs(x int) int {
	if x < 0 {
		return -x
	}
	return x
}

// On each axis, check to see if the centers of the boxes are close enough that
// they'll intersect. If they intersect on both axes, then the boxes intersect.
// If they don't, then they don't.
//
// source: http://gamedev.stackexchange.com/questions/586/what-is-the-fastest-way-to-work-out-2d-bounding-box-intersection
func isCollisionSize(aX int, aY int, aSize int, bX int, bY int, bSize int) bool {
	return (Abs(aX-bX)*2 < (aSize + bSize)) &&
		(Abs(aY-bY)*2 < (aSize + bSize))
}

// appendCollision appends the coordinates from a collison to a collison-list
func (world *World) appendCollision(x int, y int) {
	collision := new(Collision)
	collision.X = x
	collision.Y = y

	world.Collisions = append(world.Collisions, collision)
}

//removeCollisons removes all collisons from the collison-list
func (world *World) removeCollisions() {
	world.Collisions = make([]*Collision, 0)
}

// print all players and asteroid that has collided
func (world *World) print() {
	var deadPlayerIDs []int
	var deadAsteroidIDs []int

	for _, player := range world.Players {
		if player.Alive == false {
			deadPlayerIDs = append(deadPlayerIDs, player.ID)
		}
	}

	for _, asteroid := range world.Asteroids {
		if asteroid.Alive == false {
			deadAsteroidIDs = append(deadAsteroidIDs, asteroid.ID)
		}
	}

	if len(deadPlayerIDs) > 2 || len(deadAsteroidIDs) > 0 {
		DebugPrint(fmt.Sprintln("[COL.MAN] Collisions, Players:", deadPlayerIDs,
			"Asteroids:", deadAsteroidIDs))
	}
}
