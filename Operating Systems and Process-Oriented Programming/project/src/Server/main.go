package main

import (
	"fmt"
	"os"

	"./asteroids"
)

const (
	serverInacitivityLimitReal = 60
	serverInacitivityLimitFake = 5
)

func main() {

	greetUser()

	inDebugMode := setDebugMode()
	inFakeMode := setFakeMode()
	inactivityLimit := setInactivityLimit(inFakeMode)

	var server = asteroids.CreateServer(inDebugMode, inactivityLimit)

	if inFakeMode == true {
		server.Listen(server.CreateFakeUser())
	} else {
		server.Listen(make(chan asteroids.Data))
	}

	asteroids.DebugPrint(fmt.Sprintln("[MAIN] Dead"))

}

func greetUser() {
	fmt.Println("\nWelcome to the back-end of Asteroids 1.0")
	fmt.Println("Server starting…")
	fmt.Print("Parameters: ")
}

func setDebugMode() bool {

	argsWithoutProg := os.Args[1:]
	if len(argsWithoutProg) > 0 && argsWithoutProg[0] == "-s" {
		fmt.Print("Default, silent\n\n")
		return false
	}

	return true

}

func setFakeMode() bool {

	argsWithoutProg := os.Args[1:]
	if len(argsWithoutProg) > 0 && argsWithoutProg[0] == "-f" {
		fmt.Print("Fake users, verbose\n\n")
		return true
	}

	return false

}

func setInactivityLimit(inFakeMode bool) int {

	if !inFakeMode {
		return serverInacitivityLimitReal
	}

	return serverInacitivityLimitFake

}
