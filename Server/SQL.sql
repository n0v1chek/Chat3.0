
-- ================== СОЗДАНИЕ ТАБЛИЦ БД ================

-- ПОЛЬЗОВАТЕЛИ
CREATE TABLE `tusers`
(
`id`          INTEGER NOT NULL PRIMARY KEY  AUTOINCREMENT,-- код пользователя
`u_login`     VARCHAR(30) NOT NULL UNIQUE,-- логин
`u_pass`      VARCHAR(30) NOT NULL-- пароль
);



-- ОБЩИЙ ЧАТ
CREATE TABLE `tchat`
(
`id`          INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,-- код сообщения
`ch_sender`   VARCHAR(30) NOT NULL,-- логин отправителя
`ch_time`     DATETIME NOT NULL,-- время отправки сообщения
`ch_msg`      TEXT NOT NULL-- сообщение
);


-- ПРИВАТНЫЙ ЧАТ
CREATE TABLE `tprivatechat`
(
`id`          INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,-- код сообщения
`ch_sender`   VARCHAR(30) NOT NULL,-- логин отправителя
`ch_recipient`   VARCHAR(30) NOT NULL,-- логин получателя
`ch_time`     DATETIME NOT NULL,-- время отправки сообщения
`ch_msg`      TEXT NOT NULL-- сообщение
);


 
-- ================== ЗАПОЛНЕНИЕ ТАБЛИЦ БД ================

-- ПОЛЬЗОВАТЕЛИ
INSERT INTO tusers (u_login, u_pass)
VALUES
('user1', '123456'),
('user2', '123456')


-- ОБЩИЙ ЧАТ
INSERT INTO tchat (ch_sender, ch_time, ch_msg)
VALUES
('user1', DATETIME(), 'Приветствую всех! =)')







DELETE FROM tchat



