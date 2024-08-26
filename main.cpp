#include <iostream>
#include <pqxx/pqxx>

class Database
{

    public:
        Database(pqxx::connection& conn) : _connection(conn) {}

        // метод для регистрации пользователя
        void createUser(const std::string& username, const std::string& password, double initial_amount) {
            try {
                pqxx::work txn(_connection);

                // добавляем пользователя в бд
                txn.exec0("INSERT INTO users (username, password, balance) VALUES (" +
                          txn.quote(username) + ", " +
                          txn.quote(password) + ", " +
                          txn.quote(initial_amount) + ")");
                txn.commit();
                std::cout << "Пользователь успешно создан." << std::endl;
            } catch (const std::exception &e) {
                std::cerr << "Ошибка при создании пользователя: " << e.what() << std::endl;
            }
        }

    private:
        pqxx::connection& _connection;
};

int main()
{
    try {
        // добавьте свои данные для подключения к бд
        //(также понадобится таблица users с id(PK), username, password, balance)
        pqxx::connection DB("dbname=# user=# password=#");

        if (DB.is_open()) {
            std::cout << "Соединение с базой данных установлено." << std::endl;

            Database database(DB); // подключаем бд

            int choice;
            while (true) {
                std::cout << "Меню команд:" << std::endl;
                std::cout << "1 - Зарегистрироваться" << std::endl;
                std::cout << "0 - Выход" << std::endl;
                std::cout << "Введите команду: ";
                std::cin >> choice;

                if (choice == 1) {
                    std::string username, password;
                    double initial_amount;
                    std::cout << "Введите имя пользователя: ";
                    std::cin >> username;
                    std::cout << "Введите пароль: ";
                    std::cin >> password;

                    database.createUser(username, password, 0);
                } else if (choice == 0) {
                    break;
                } else {
                    std::cout << "Неверная команда, попробуйте еще раз." << std::endl;
                }
            }

        } else {
            std::cout << "Не удалось подсоединиться." << std::endl;
            return 1;
        }


    } catch (const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}




