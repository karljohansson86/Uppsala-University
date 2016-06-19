package asteroids

import (
	"encoding/json"
	"fmt"
	"time"
)

// Data is a semi-generic struct to be sent in channels
type Data struct {
	action string
	result int
}

// Connection struct, containing one write and one read channel
type Connection struct {
	write chan Data
	read  chan Data
}

// gameSession is a local copy of a session, to be used for handling where to connect new players
type gameSession struct {
	id int
	*Connection
}

// Server struct used to store information about sessions, players etc.
type Server struct {
	totalPlayers    int
	nextPort        int
	maxPlayers      int
	nextSession     int
	inactivityLimit int
	input           chan Data
	sessions        []*gameSession
}

// Some signal constants
const terminate = "kill"
const terminated = "dead"
const request = 100
const fail = -1
const ok = 200

// inDebugMode used when running program with prints
var inDebugMode = false

// DebugPrint prints the argument string, used to have same output in complete program
func DebugPrint(str string) {

	if inDebugMode {
		fmt.Print("* ", str)
	}

}

// MakeConnection makes a Connection struct with channels of type data
func MakeConnection() *Connection {

	conn := new(Connection)
	conn.read = make(chan Data)
	conn.write = make(chan Data)
	return conn

}

// FlipConnection flips the outlets of a Connection struct
func (conn *Connection) FlipConnection() *Connection {

	flipped := new(Connection)
	flipped.read = conn.write
	flipped.write = conn.read
	return flipped

}

// CreateFakeUser is a temporary/debug function to create a mock user
func (server *Server) CreateFakeUser() chan Data {

	fakeUser := make(chan Data)
	nextPort := server.nextPort

	go func() {

		fakeUser <- Data{"server.new_user", nextPort}
		<-fakeUser

		fakeUser <- Data{"server.new_user", nextPort}
		<-fakeUser

	}()

	return fakeUser
}

// Listen is a loop that server uses to listen for and setup new users
func (server *Server) Listen(external chan Data) {

	server.input = external

	// Kill the server after x seconds of inactivity
	timeout := time.After(time.Duration(server.inactivityLimit) * time.Second)

	//newPlayers := make(chan int)
	//go server.acceptNewPlayers(newPlayers)

	for {
		select {
		case msg := <-server.input:

			if msg.action == "server.new_user" {
				port := server.addPlayer()
				external <- Data{"port", port}
				DebugPrint(fmt.Sprintln("[SERVER] Port set up for new user", port))
			}

			if msg.action == terminate {
				DebugPrint(fmt.Sprintln("\nEXITING…\n========"))
				server.killSessions()
				DebugPrint(fmt.Sprintln("[SERVER] Dead"))
				return
			}
			// newPlayers <- port

		case <-timeout:
			server.kill()
		}
	}

}

// TODO check this function, right now no call from listen to this one
// acceptNewPlayers is used with a seperate go-routine waiting/ checking if any
// new users want to connect
func (server *Server) acceptNewPlayers(conn chan int) {

	socket, err := CreateSocket(9000)
	if err != nil {
		panic(err)
	}

	for {
		tcpConn, err := socket.Accept()
		if err != nil {
			panic(err)
		}

		conn <- 100
		fmt.Println("Sent request to server")
		portData := <-conn
		jsonPort, err := json.Marshal(&portData)

		if err != nil {
			panic(err)
		}
		tcpConn.Write(jsonPort)

		tcpConn.Close()
	}
}

// addPlayer adds a player to first available session that has capacity for a new player
func (server *Server) addPlayer() int {

	for _, session := range server.sessions {

		// Ask a session whether there is enough room for a new player
		session.write <- Data{"server.poke", request}
		port := <-session.read

		if port.result > -1 {
			return server.createPlayer(session)
		}
	}

	// If no session has capacity or available, creates a new session and player
	return server.createSession()

}

// CreateServer creates a new server struct
func CreateServer(debugMode bool, inactivityLimit int) *Server {

	inDebugMode = debugMode

	server := new(Server)
	server.maxPlayers = 8
	server.totalPlayers = 0
	server.inactivityLimit = inactivityLimit

	/*
		9001 because it allows us to count acceptNewPlayers
		based on the ports, and also gives us 9000 as static server port
	*/
	server.nextPort = 9001

	return server

}

func (server *Server) kill() {

	go func() {
		server.input <- Data{terminate, request}
	}()

}

func (server *Server) killSessions() {

	for _, session := range server.sessions {
		session.write <- Data{terminate, request}
		<-session.read
	}

}

// createSession creates a local copy of the session in the server
func (server *Server) createSession() int {

	//serverSide, sessionSide := makeTwoWayConnection()
	sessConn := MakeConnection()
	nextPort := server.getNextPort()

	// Start a session and wait for it to send confirmation
	go Session(sessConn.FlipConnection(), nextPort, server.maxPlayers, 1080, 1920)
	<-sessConn.read

	DebugPrint(fmt.Sprintln("[SERVER] Session created"))

	// Create a local fake-session to track some basic stats
	session := new(gameSession)

	session.Connection = sessConn
	session.id = server.getNextSessionID()
	server.sessions = append(server.sessions, session)

	return server.createPlayer(session)

}

// createPlayer sends request to session to create a new player and waits for confirmation
func (server *Server) createPlayer(gs *gameSession) int {

	gs.write <- Data{"server.connect_player", request}
	data := <-gs.read

	DebugPrint(fmt.Sprintln("[SERVER] Player connected"))

	server.totalPlayers++
	return data.result
}

// getNextPort returns the next start port to be used by a session
func (server *Server) getNextPort() int {
	defer func() { server.nextPort += server.maxPlayers }()
	return server.nextPort
}

// getNextSessionID returns the next session id to be used by a session
func (server *Server) getNextSessionID() int {
	defer func() { server.nextSession++ }()
	return server.nextSession
}
