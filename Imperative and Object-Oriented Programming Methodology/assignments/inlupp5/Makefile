
JFLAGS = -g -Xlint:unchecked #-g generate debugging information , Xlint for warnings
JC = javac


JUNITCOMP = -cp .:junit-4.12.jar
JUNITRUN = -cp .:junit-4.12.jar:hamcrest-core-1.3.jar org.junit.runner.JUnitCore

all:
	@$(JC) $(JFLAGS) *[!Test].java

run: all
	@java Main 

docs: 
	@javadoc -d docs Book.java Course.java CreateWorld.java Creature.java Game.java Item.java Key.java Player.java Qanda.java Room.java Sfinx.java Student.java Teacher.java

test:
	@$(JC) $(JFLAGS) $(JUNITCOMP) *Test.java
	@java $(JUNITRUN) GameTest PlayerTest StudentTest

clean:
	rm -f *~
	rm -f *.class
	rm -f *#
	rm -rf docs/

