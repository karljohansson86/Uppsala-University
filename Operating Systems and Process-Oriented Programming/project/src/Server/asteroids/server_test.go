package asteroids

import (
	"testing"
	"time"
)

var startPort = 9001
var maxPlayers = 8

func TestCreateServer(t *testing.T) {

	server := CreateServer(false, 5)

	if server.totalPlayers != 0 {
		t.Error("Expected totalPlayers 0, got", server.totalPlayers)
	}

	if server.maxPlayers != maxPlayers {
		t.Error("Expected maxPlayers", maxPlayers, "\b, got", server.maxPlayers)
	}

	if server.nextPort != startPort {
		t.Error("Expected nextPort", startPort, "\b, got", server.nextPort)
	}

}

func TestGetNextPort(t *testing.T) {

	server := CreateServer(false, 5)
	// Make sure server is up
	time.Sleep(100 * time.Millisecond)

	if server.nextPort != startPort {
		t.Error("Expected nextPort", startPort, "\b, got", server.nextPort)
	}
	nextPort := server.getNextPort()
	if nextPort != startPort {
		t.Error("Expected nextPort", startPort, "\b, got", nextPort)
	}
	if server.nextPort != startPort+maxPlayers {
		t.Error("Expected nextPort", startPort+maxPlayers, "\b, got", server.nextPort)
	}

}

func TestGetNextSessionID(t *testing.T) {

	server := CreateServer(false, 5)
	// Make sure server is up
	time.Sleep(100 * time.Millisecond)

	if server.nextSession != 0 {
		t.Error("Expected nextSession", 0, "\b, got", server.nextSession)
	}
	nextSession := server.getNextSessionID()
	if nextSession != 0 {
		t.Error("Expected nextSession", 0, "\b, got", nextSession)
	}
	if server.nextSession != 1 {
		t.Error("Expected nextSession", 1, "\b, got", server.nextSession)
	}

}

func TestMakeConnection(t *testing.T) {

	cInternal := MakeConnection()
	cExternal := cInternal.FlipConnection()

	// cInternal writes to cExternal's read and vice versea

	go func() {

		cInternal.write <- Data{"int_to_ext", 200}
		cExternal.write <- Data{"ext_to_int", 200}

	}()

	val1 := <-cExternal.read
	val2 := <-cInternal.read

	if val1.action != "int_to_ext" {
		t.Error("Expected int_to_ext, got", val1.action)
	}

	if val2.action != "ext_to_int" {
		t.Error("Expected ext_to_int, got", val1.action)
	}

}

func TestAddPlayer(t *testing.T) {

	server := CreateServer(false, 5)

	p1port := server.addPlayer()
	p2port := server.addPlayer()

	if p1port != startPort {
		t.Error("Expected", startPort, "\b, got", p1port)
	}

	if p2port != startPort+1 {
		t.Error("Expected", startPort+1, "\b, got", p2port)
	}

}
