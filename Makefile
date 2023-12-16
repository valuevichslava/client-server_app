.PHONY: build-all run-server1 run-server2 run-client

build-all:  
	@docker network create mynetwork || true
	@cd client && docker build -t nclient . 
	@cd server1 && docker build -t nserver1 .
	@cd server2 && docker build -t nserver2 .

run-server1:
	@docker rm -f nserver1 || true
	@docker run -d --name nserver1 --network=mynetwork --ip 10.89.0.2 -p 5001:5001 nserver1

run-server2:
	@docker rm -f nserver2 || true
	@docker run -d --name nserver2 --network=mynetwork --ip 10.89.0.3 -p 5002:5002 nserver2

run-client:
	@docker rm -f nclient || true
	@docker run -d --name nclient --network=mynetwork nclient
