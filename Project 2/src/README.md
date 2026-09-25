# CSE4110 / 데이터베이스시스템 / Project2
**20201444 김민서**

## 관련 파일
- source file: main.cpp
- sql file: schema.sql, sample_data.sql

## 실행 환경
- OS: Windows11
- Database: MySQL 8.0.42
- Design Tool: MySQL Workbench
- Programming Language: C++
- Compiler: MSVC / Visual Studio Code 1.101.0
- Database Connectivity: MsSQL C API
- MySQL Connector C 6.1 사용 (다운로드 필수)
- x64 Native Tools Command Prompt for VS 2022 사용

## Code 설명
코드는 크게 displayMenu(), executeQuery(), main()으로 구성되어 있다.
- displayMenu()
  - 시작 interface를 출력하는 부분이다. 사용자가 TYPE 1 ~ 7까지 고르도록 안내하는 역할을 한다.
  - main() 함수의 while문 내에서 executeQuery()의 처리 이전에 계속 호출된다.
- executeQuery()
  - 주어진 쿼리를 처리하는 함수이다.
  - 해당 함수 내부에서는 '쿼리 실행 -> 결과 저장 -> 필드 개수 가져오기 -> 헤더 출력 -> 행 출력' 순으로 처리된다.
- main()
  - 메인 함수로 displayMenu() 이후 사용자의 입력을 받고, 이를 토대로 switch문으로 적절한 작업을 처리하게 한다.
  - 이때 사용자의 선택(1 ~ 7)에 따라 각 case 내에서는 알맞은 쿼리가 지정되어 executeQuery()를 호출한다.
  - 0을 선택하면 프로그램이 종료된다.
- 기존 코드 변경 사항
  - #include <mysql/mysql.h> --> #include <mysql.h>

## 실행 방법
아래의 과정에 따라 main.cpp를 실행시킨다.
1. MySQL 서버를 실행시킨다. (server = 'localhost', user = 'root', password = '1234', database = 'store')
2. 실행 중인 서버에 접속하여 schema.sql, sample_data.sql을 차례대로 실행시킨다.
   - 이때 SCHEMAS에 store를 생성하고, 더블 클릭하여 해당 스키마를 기본 DB로 설정해야 한다.
   - 필요한 경우에 schema.sql의 맨앞에 있는 drop 부분을 주석처리해야 한다.
3. x64 Native Tools Command Prompt for VS 2022를 실행시키고 아래 명령어를 입력한다.
   ```
   code .
   ```
5. vscode에 접속이 되는데, 여기에서 main.cpp가 위치한 디렉토리로 이동한다.
6. `mysql -u root -p`를 입력한다.
7. 비밀번호를 입력한다. (비밀번호 = 1234)
8. 아래 명령어를 차례대로 입력하고 `exit`한다.
   ```
   ALTER USER 'root'@'localhost' IDENTIFIED WITH mysql_native_password BY '1234';
   FLUSH PRIVILEGES;
   ```
9. 8까지 끝난 이후 아래의 명령어를 입력한다.
   - 명령어가 실행이 안되는 경우에는 파일의 경로를 잘 확인하여 수정할 필요가 있다.
   ```
   cl.exe /EHsc /I"C:\Program Files\MySQL\MySQL Connector C 6.1\include" main.cpp /link /LIBPATH:"C:\Program Files\MySQL\MySQL Connector C 6.1\lib" libmysql.lib
   ```
10. ls를 통해 생성된 main.exe를 확인할 수 있다. 해당 파일은 아래의 명령어로 실행시킬 수 있다.
    ```
     ./main.exe
    ```

## Query 테스트 방법
main.exe를 실행시키면 시작 Interface가 나타난다.
사용자는 0 ~ 7까지의 입력을 통해 쿼리문의 결과 확인 및 프로그램 종료 등의 동작을 지시할 수 있다.
- **0**: 프로그램 종료
- **1 ~ 7**: 쿼리문 실행 및 결과 확인
    - **1**: which stores currently carry a certain product (by UPC, name, or brand), and how much inventory do they have?
    - **2**: which products have the highest sales volume in each store over the past month?
    - **3**: which store has generated the highest overall revenue this quarter?
    - **4**: which vendor supplies the most products across the chain, and how many total units have been sold?
    - **5**: which products in each store are below the reorder threshold and need restocking?
    - **6**: List the top 3 items that loyalty program customers typically purchase with coffee.
    - **7**: Among franchise-owned stores, which one offers the widest variety of products, and how does that compare to corporate-owned stores?
