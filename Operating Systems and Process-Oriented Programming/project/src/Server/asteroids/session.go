package asteroids

import (
	"fmt"
	"sync"
	"time"
)

// World is a placeholder for the gameboard
//type World int
// TODO: CHANGE THIS
type World struct {
	width      int
	height     int
	Players    []*Player
	Asteroids  []*Asteroid
	Collisions []*Collision
}

// Collision holds the coordinates of a collision
type Collision struct {
	X int
	Y int
}

// channels struct used to implement a structured way to handle multiple
// write/read channels for session
type channels struct {
	server    chan (Data)
	players   chan (Data)
	asteroids chan (Data)
}

// session struct stores info regarding players,session managers,
// read/write channels etc.
type session struct {
	worldSize       int
	currentPlayers  int
	maxPlayers      int
	world           *World
	asteroidManager *asteroidManager
	listenerManager *ListenerManager
	// For external communication
	write channels
	read  channels
}

var mutex sync.Mutex

// Session initiates the sessions values and creates a new go-routine for the session
func Session(serverConn *Connection, startPort int, players int, worldHeight int, worldWidth int) {
	mutex = sync.Mutex{}
	session := new(session)
	session.maxPlayers = players

	session.world = new(World)
	session.world.height = worldHeight
	session.world.width = worldWidth
	session.write.server = serverConn.write
	session.read.server = serverConn.read

	session.write.server <- Data{"session_created", ok}

	session.createManagers(startPort)

	go session.loop()

}

func (session *session) kill() {

	go func() {
		session.read.server <- Data{terminate, request}
	}()

}

func (session *session) killManagers() {

	// Kill listener manager and wait for confirmation
	session.listenerManager.kill()
	<-session.read.players

	// Kill asteroid manager and wait for confirmation
	session.asteroidManager.kill()
	<-session.read.asteroids

}

// loop is the sessions game loop, sending ticks to it's managers and collecting data and
// calculating collisions and distribute the new world
func (session *session) loop() {

	for {

		tick := time.After(16 * time.Millisecond)

		select {
		case <-tick:
			// Collect player and asteroid positions
			session.world.Players = session.listenerManager.getPlayers()
			session.world.Asteroids = session.asteroidManager.getAsteroids()

			// Calculate collisions
			session.world.collisionManager()

			// BROADCAST TO CLIENTS
			/*var list []int
			for _, ass := range session.world.Asteroids {
				list = append(list, ass.ID)
			}*/

			//fmt.Println("SES:", session.world.Asteroids)
			//fmt.Println("SES:", list)
			session.listenerManager.sendToClient(session.world)

			//session.world.Players[0].fakeMovePlayer()
			//Faking player movement so that I have something to draw

			//Empty world {}, something is going wrong.
			//session.world.players jsons fine, but world just doesn't

			session.write.asteroids <- Data{"session.tick", ok}
			session.write.players <- Data{"session.tick", ok}

		case msg := <-session.read.server:

			if msg.action == "server.poke" {

				// Check if theres room inside the session
				if session.currentPlayers < session.maxPlayers {
					session.write.server <- Data{"session.has_room", ok}
				} else {
					session.write.server <- Data{"session.no_room", fail}
				}

			} else if msg.action == terminate {

				session.killManagers()
				DebugPrint(fmt.Sprintln("[SESSION] Dead"))
				session.write.server <- Data{terminated, ok}
				return

			} else {

				// Spawn a new player
				var port, player = session.listenerManager.newPlayer()
				session.currentPlayers++
				session.world.Players = append(session.world.Players, player)

				session.write.server <- Data{"session.player_created", port}

			}
		}
	}
}

// createManagers sets up managers and their respective connections to/from session
func (session *session) createManagers(startPort int /*maxPlayers int, maxAsteroids int*/) {

	toPlayers := MakeConnection()
	session.write.players = toPlayers.write
	session.read.players = toPlayers.read

	toAsteroids := MakeConnection()
	session.write.asteroids = toAsteroids.write
	session.read.asteroids = toAsteroids.read

	session.asteroidManager = newAsteroidManager()
	session.listenerManager = newListenerManager()

	go session.asteroidManager.loop(toAsteroids.FlipConnection(), session.world.height, session.world.width)
	go session.listenerManager.loop(toPlayers.FlipConnection(),
		session.maxPlayers, startPort)

	// Wait for managers to signal that they are ready
	<-toAsteroids.read
	<-toPlayers.read

}
