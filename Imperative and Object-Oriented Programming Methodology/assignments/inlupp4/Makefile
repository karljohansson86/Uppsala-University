JFLAGS = -g -Xlint:unchecked
JC = javac

JUNITCOMP = -cp .:junit-4.12.jar
JUNITRUN = -cp .:junit-4.12.jar:hamcrest-core-1.3.jar org.junit.runner.JUnitCore

CBLUE = \x1B[34m
CGREEN = \x1B[32m
CRESET = \x1B[0m

.PHONY: docs

all:
	@$(JC) $(JFLAGS) *[!Test].java

run:
	@java Simulator

def:
	@java Simulator --d

docs:
	@javadoc -d docs Customer.java Register.java Queue.java \
	LinkedQueue.java Store.java Simulation.java

clean:
	rm -f *~
	rm -f *#
	rm -f *.gch
	rm -f *.class

test: testmsg all testcomp CustomerTest QueueTest RegisterTest StoreTest
	@echo "${CGREEN}All tests completed successfully\n${CRESET}"

testmsg: 
	@printf "\nCompiling…"

testcomp:
	@$(JC) $(JFLAGS) $(JUNITCOMP) *Test.java
	@echo "\n"

%Test: %Test.java
	@printf "${CBLUE}Running $@\n${CRESET}"
	@java $(JUNITRUN) $@

