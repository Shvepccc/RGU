-- Создание таблицы видов отчетности
CREATE TABLE reporting_type (
    id SERIAL PRIMARY KEY,
    type_name VARCHAR(20) UNIQUE NOT NULL
);

-- Вставка основных типов отчетности
INSERT INTO reporting_type (type_name) VALUES 
    ('rating'),
    ('exam'),
    ('paper');

-- Создание таблицы групп
CREATE TABLE student_group (
    id SERIAL PRIMARY KEY,
    code VARCHAR(50),
    course INT
);

-- Создание таблицы студентов
CREATE TABLE student (
    id SERIAL PRIMARY KEY,
    last_name VARCHAR(50),
    first_name VARCHAR(50),
    middle_name VARCHAR(50),
    gender CHAR(1),
    birth_date DATE,
    group_id INT REFERENCES student_group(id)
);

-- Создание таблицы предметов
CREATE TABLE subject (
    id SERIAL PRIMARY KEY,
    name VARCHAR(100),
    reporting_type_id INT REFERENCES reporting_type(id)
);

-- Создание таблицы оценок
CREATE TABLE marks (
    stud_id INT REFERENCES student(id),
    subj_id INT REFERENCES subject(id),
    semester_number INT CHECK (semester_number BETWEEN 1 AND 8),
    grade INT CHECK (grade BETWEEN 2 AND 5),
    PRIMARY KEY (stud_id, subj_id, semester_number)
);




-- Функция для генерации случайных данных
CREATE OR REPLACE FUNCTION generate_test_data()
RETURNS void AS $$
DECLARE
    i INT;
    group_id INT;
    student_id INT;
    subject_id INT;
    reporting_type_count INT;
BEGIN
    -- Очистка таблиц (в обратном порядке из-за внешних ключей)
    TRUNCATE TABLE marks, subject, student, student_group, reporting_type RESTART IDENTITY CASCADE;

    -- Заполнение таблицы видов отчетности
    INSERT INTO reporting_type (type_name) VALUES 
        ('rating'),
        ('exam'),
        ('paper');
    
    -- Генерация групп
    FOR i IN 1..10 LOOP
        INSERT INTO student_group (code, course) 
        VALUES ('GROUP-' || i, (random() * 3 + 1)::INT);
    END LOOP;

    -- Генерация студентов (по 20-30 в каждой группе)
    FOR group_id IN 1..10 LOOP
        FOR i IN 1..(20 + random() * 10)::INT LOOP
            INSERT INTO student (last_name, first_name, middle_name, gender, birth_date, group_id)
            VALUES (
                'LastName_' || group_id || '_' || i,
                'FirstName_' || group_id || '_' || i,
                'MiddleName_' || group_id || '_' || i,
                CASE WHEN random() > 0.5 THEN 'M' ELSE 'F' END,
                DATE '2000-01-01' + (random() * 2000)::INT,
                group_id
            );
        END LOOP;
    END LOOP;

    -- Генерация предметов
    FOR i IN 1..25 LOOP
        INSERT INTO subject (name, reporting_type_id)
        VALUES (
            'Subject_' || i,
            (random() * 2 + 1)::INT
        );
    END LOOP;

    -- Генерация оценок (каждый студент получает оценки по 5-15 предметам за разные семестры)
    SELECT COUNT(*) INTO reporting_type_count FROM reporting_type;
    
    FOR student_id IN 1..(SELECT COUNT(*) FROM student) LOOP
        FOR i IN 1..(5 + random() * 10)::INT LOOP
            subject_id := (random() * 24 + 1)::INT;
            
            INSERT INTO marks (stud_id, subj_id, semester_number, grade)
            VALUES (
                student_id,
                subject_id,
                (random() * 7 + 1)::INT,
                CASE 
                    WHEN random() > 0.1 THEN (random() * 2 + 3)::INT  -- 3-5 с вероятностью 90%
                    ELSE 2  -- 2 с вероятностью 10%
                END
            )
            ON CONFLICT (stud_id, subj_id, semester_number) DO NOTHING;
        END LOOP;
    END LOOP;

    RAISE NOTICE 'Тестовые данные успешно сгенерированы!';
END;
$$ LANGUAGE plpgsql;

-- Вызов функции для генерации данных
SELECT generate_test_data();