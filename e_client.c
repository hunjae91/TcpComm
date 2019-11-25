#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#define BUF_SIZE 1024
void error_handling(char * message);
int main(int argc, char *argv[])
{
	int sock; // 서버에 접속할 소켓
	char message[BUF_SIZE]; // 서버 보낼 메세지를 저장할 문자열
	int str_len; // 송수신 메세지의 문자열 길이
	struct sockaddr_in serv_adr; // 접속할 서버의 주소(어딘지 미리알아야됨)
	// IP와 Port를 지정 하지 않았으면 사용법을 알려준다.
	if(argc!=3){
		printf("Usage : %s <IP> <port>\n", argv[0]); // 사용법 표시
		exit(1); // 비정상 종료
	}

	sock = socket(PF_INET, SOCK_STREAM, 0); // TCP(STREAM) 프로토콜 생성
	if(sock == -1) // 안만들어 지면
		error_handling("socket() error"); // 에러 표시후 종료

	memset(&serv_adr, 0, sizeof(serv_adr)); // 서버쪽 주소 구조체 0으로 초기화
	serv_adr.sin_family = AF_INET; // 인터넷 주소체계 사용
	serv_adr.sin_addr.s_addr = inet_addr(argv[1]); // 서버 IP를 구조체에 저장한다.
	serv_adr.sin_port = htons(atoi(argv[2])); // 서버 Port를 구조체에 저장한다.

	// connect는 클라이언트 전용함수 이고, 서버는 accept 상태여야한다.
	if(connect(sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1)
		error_handling("connect() error!"); // 연결실패
	else
		puts("Connected......"); // 연결 성공

	while(1) // 연결되고 나면 무한루프 진입, 언제 끝나느냐?
	{
		fputs("Input message(Q to quit) : ", stdout); // 메세지 입력 안내
		fgets(message, BUF_SIZE, stdin); // 사용자로부터 메세지를 입력받음

		if(!strcmp(message, "q\n") || !strcmp(message, "Q\n"))
			break;
		write(sock, message, strlen(message)); // 소켓을 이용해 메세지를 전송
		str_len = read(sock, message, BUF_SIZE-1); // 메아리되어 돌아오는 메세지 수신
		message[str_len] = 0; // 보낼때 NULL문자 안보내서, 받았을때 넣어줌
		printf("Message from server : %s", message);
	}

	close(sock); // 종료하기 전에 시스템에서 얻어온 소켓을 돌려준다.
	return 0;
}

void error_handling(char * message)
{
	fputs(message, stderr);
	fputc('\n', stderr);
	exit(1);
}

