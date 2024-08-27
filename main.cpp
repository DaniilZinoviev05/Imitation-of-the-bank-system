#include <iostream>
#include <pqxx/pqxx>

class Database
{

    public:
        Database(pqxx::connection& conn) : _connection(conn) {}

        // метод для регистрации пользователя
        void createUser(const std::string& phone_number, const std::string& username, const std::string& password, double initial_amount)
        {
            try
            {
                pqxx::work txn(_connection);
                // добавляем пользователя в бд
                txn.exec0("INSERT INTO users (phone_number, username, password, balance) VALUES (" +
                          txn.quote(phone_number) + ", " +
                          txn.quote(username) + ", " +
                          txn.quote(password) + ", " +
                          txn.quote(initial_amount) + ")");
                txn.commit();
                std::cout << "Пользователь успешно создан." << std::endl;
            }
            catch (const std::exception &e)
            {
                std::cerr << "Ошибка при создании пользователя: " << e.what() << std::endl;
            }
        }

        bool checkUser(const std::string& phone_number)
        {
            try
            {
                pqxx::work txn(_connection);
                pqxx::result result = txn.exec("SELECT * FROM users WHERE phone_number = " + txn.quote(phone_number));
                txn.commit();

                if (result.size() > 0)
                {
                    std::cout << "Пользователь найден." << std::endl;
                    return true;
                }
                else
                {
                    std::cout << "Пользователь не найден." << std::endl;
                    return false;
                }
            }

            catch (const std::exception &e)
            {
                std::cerr << "Ошибка при поиске: " << e.what() << std::endl;
            }

            return false;
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
            while (true)
            {
                std::cout << "Меню команд:" << std::endl;
                std::cout << "1 - Зарегистрироваться" << std::endl;
                std::cout << "2 - Логин" << std::endl;
                std::cout << "0 - Выход" << std::endl;
                std::cout << "Введите команду: ";
                std::cin >> choice;

                if (choice == 1)
                {
                    std::string phone_number, username, password;
                    double initial_amount;
                    std::cout << "Введите номер телефона: ";
                    std::cin >> phone_number;
                    std::cout << "Введите имя пользователя: ";
                    std::cin >> username;
                    std::cout << "Введите пароль: ";
                    std::cin >> password;

                    database.createUser(phone_number, username, password, 0);
                }

                else if (choice == 2)
                {
                    std::string phone_number, password;
                    double initial_amount;
                    std::cout << "Введите номер телефона: ";
                    std::cin >> phone_number;

                    if (database.checkUser(phone_number))
                    {
                        std::cout << "Введите пароль: ";
                        std::cin >> password;
                        while (true)
                        {
                            std::cout << "Добро пожаловать в личный кабинет." << std::endl;
                            std::cout << "Меню команд:" << std::endl;
                            std::cout << "1 - Пополнить баланс" << std::endl;
                            std::cout << "2 - Перевести деньги" << std::endl;
                            std::cout << "0 - Выход" << std::endl;
                            std::cout << "Введите команду: ";
                            std::cin >> choice;
                            if (choice == 1)
                            {

                            }
                        }
                    }
                }

                else if (choice == 0)
                {
                    break;
                }

                else
                {
                    std::cout << "Неверная команда, попробуйте еще раз." << std::endl;
                }
            }

        }
        else
        {
            std::cout << "Не удалось подсоединиться." << std::endl;
            return 1;
        }


    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}




