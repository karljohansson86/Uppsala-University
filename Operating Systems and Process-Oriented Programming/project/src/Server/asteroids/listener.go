package asteroids

import (
	"encoding/json"
	//"fmt"
	"math/rand"
	"net"
	"strconv"
	"time"
)

//Player is used to represent the players in the game world
type Player struct {
	Name  string
	ID    int
	X     int
	Y     int
	size  int
	Lives int
	Alive bool
}

//Listener is responsible for a client each
//Contains a tcp socket, with the specified port at creation
type Listener struct {
	socket      net.Listener
	ID          string
	port        int
	player      *Player
	conn        net.Conn
	writeBuffer chan []byte
}

//CreateSocket creates a tcp listener at the specified port
func CreateSocket(port int) (net.Listener, error) {

	connection, err := net.Listen("tcp", ":"+strconv.Itoa(port))
	if err != nil {
		return nil, err
	}

	return connection, nil
}

//NewListener creates a new socket then runs this socket as a go routine
func newListener() *Listener {

	return new(Listener)
}

// init initiates the listeners values
func (listener *Listener) init(port int) {

	var err error
	listener.socket, err = CreateSocket(port)
	if err != nil {
		panic(err)
	}

	listener.port = port

	/*
		This should be synchronized with listener?

		listener.player = new(Player)
		listener.player.Name = strconv.Itoa(port)
		listener.player.X = 0
		listener.player.Y = 0
		listener.player.Lives = 3
	*/

	listener.writeBuffer = make(chan []byte, 60)

}

//newPlayer returns a new player
func newPlayer() *Player {
	return new(Player)
}

//init initiates a new players values
func (player *Player) init(id int, xMax int, yMax int) {
	player.ID = id
	seed := time.Now().UnixNano()
	rand.Seed(seed)

	player.randomSpawn(xMax, yMax)
	player.size = 30
	player.Lives = 3 // updated
	player.Alive = true
}

// startUpListener
func (listener *Listener) startUpListener() {
	var err error
	listener.conn, err = listener.socket.Accept()
	if err != nil {
		panic(err)
	}
	listener.ID = "Hello World"

	listener.idleListener()
}

func (listener *Listener) idleListener() {

	/*
		for {
			if listener.writeBuffer[0] != nil {
				listener.conn.Write(listener.writeBuffer[0])
				listener.writeBuffer = listener.writeBuffer[1:]
			} else {
				time.Sleep(time.Second)
			}
		}
	*/

	for {
		select {
		case jsonWorld := <-listener.writeBuffer:
			//sizeWorld := binary.Size(jsonWorld)
			//jsonSize, err := json.Marshal(sizeWorld)
			//if err != nil {
			//	panic(err)
			//}
			//listen.conn.Write(jsonSize)
			listener.conn.Write(jsonWorld)
		default:
		}
	}
}

//write writes game world to clients
func (listener *Listener) Write(world *World) {

	//fmt.Println(world.Asteroids)
	/*var list []int
	for _, ass := range world.Asteroids {
		list = append(list, ass.ID)
	}
	fmt.Println("LIST:", list)*/

	jsonWorld, err := json.Marshal(world)
	if err != nil {
		panic(err)
	}

	listener.writeBuffer <- jsonWorld
	//listener.writeBuffer = append(listener.writeBuffer, jsonWorld)
}

// getPlayer returns a listeners player
func (listener *Listener) getPlayer() *Player {
	return listener.player
}

// randomSpawn spawn a player on a random location
func (player *Player) randomSpawn(xMax int, yMax int) {

	player.X = rand.Intn(xMax)
	player.Y = rand.Intn(yMax)
}

//isAlive return if the player is alive or not
func (player *Player) isAlive() bool {
	return player.Alive
}

//getLives returns the amount of lives the player has left
func (player *Player) getLives() int {
	return player.Lives
}

//setAlive sets the Alive state to true
func (player *Player) setAlive() {
	player.Alive = true
}
