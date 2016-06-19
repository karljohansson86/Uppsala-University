all:
	javac *.java

test:	all
	@echo "All tests should start with"
	@echo "z x y"
	@echo "z x"
	@echo "z"
	@echo "Next line should end with 'a c b'"
	@java Driver a b c
	@echo "----------------------------------"
	@echo "Next line should end with 'a a a a a'"
	@java Driver a a a a a
	@echo "----------------------------------"
	@echo "Next line should end with 'a f b e c d'"
	@java Driver  a b c d e f
	@echo "----------------------------------"
	@echo "Next line should end with 'a b c d e f'"
	@java Driver a c e f d b

clean:
	rm -f *.class
