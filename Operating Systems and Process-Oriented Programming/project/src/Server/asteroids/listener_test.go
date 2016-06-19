package asteroids

import (
	"testing"
)

func TestCreateSocket(t *testing.T) {

	_, err := CreateSocket(9000)
	if err != nil {
		t.Error("the connection was not established")
	}

}

func TestListenerInit(t *testing.T) {
	listener := newListener()

	listener.init(8000)

	if listener.port != 8000 {
		t.Error("Listener.port is wrong! Expected: 9002, Got : ", listener.port)
	}


}

func TestPlayerInit(t *testing.T) {
	player := newPlayer()

	player.init(1, 10, 10)

	if player.ID != 1 {
		t.Error("Player id is wrong: expected: 1 , Got: ", player.ID)
	}

	if player.getLives() != 3 {
		t.Error("Player Lives is wrong: expected: 3 , Got: ", player.ID)
	}

	if !player.isAlive() {
		t.Error("Player Alive is wrong: expected: true, Got: ", player.Alive)
	}

}
