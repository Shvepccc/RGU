-- 0 Clear all data
TRUNCATE TABLE 
    public.issuance,
    public.booking,
    public.book_instance,
    public.book,
    public.reader,
    public.publishing_house,
    public.author,
    public.logs
RESTART IDENTITY CASCADE;

/*
-- 18 LOGS
-- Logs function
CREATE OR REPLACE FUNCTION public.log_dml_operations()
RETURNS TRIGGER AS $$
DECLARE
    operation_text TEXT;
    old_data TEXT;
    new_data TEXT;
BEGIN

    IF TG_OP = 'INSERT' THEN
        operation_text := 'INSERT: ';
        new_data := row_to_json(NEW)::TEXT;
        INSERT INTO public.logs (log_context, log_content)
        VALUES (TG_TABLE_NAME, operation_text || ' - Data: ' || new_data);
        RETURN NEW;
        
    ELSIF TG_OP = 'UPDATE' THEN
        operation_text := 'UPDATE: ';
        old_data := row_to_json(OLD)::TEXT;
        new_data := row_to_json(NEW)::TEXT;
        INSERT INTO public.logs (log_context, log_content)
        VALUES (TG_TABLE_NAME, operation_text || ' - Old: ' || old_data || ' - New: ' || new_data);
        RETURN NEW;
        
    ELSIF TG_OP = 'DELETE' THEN
        operation_text := 'DELETE: ';
        old_data := row_to_json(OLD)::TEXT;
        INSERT INTO public.logs (log_context, log_content)
        VALUES (TG_TABLE_NAME, operation_text || ' - Data: ' || old_data);
        RETURN OLD;
    END IF;
    
    RETURN NULL;
END;
$$ LANGUAGE plpgsql;

-- Logs triggers
CREATE TRIGGER trg_log_author
    AFTER INSERT OR UPDATE OR DELETE ON public.author
    FOR EACH ROW EXECUTE FUNCTION public.log_dml_operations();

CREATE TRIGGER trg_log_publishing_house
    AFTER INSERT OR UPDATE OR DELETE ON public.publishing_house
    FOR EACH ROW EXECUTE FUNCTION public.log_dml_operations();

CREATE TRIGGER trg_log_book
    AFTER INSERT OR UPDATE OR DELETE ON public.book
    FOR EACH ROW EXECUTE FUNCTION public.log_dml_operations();

CREATE TRIGGER trg_log_reader
    AFTER INSERT OR UPDATE OR DELETE ON public.reader
    FOR EACH ROW EXECUTE FUNCTION public.log_dml_operations();

CREATE TRIGGER trg_log_book_instance
    AFTER INSERT OR UPDATE OR DELETE ON public.book_instance
    FOR EACH ROW EXECUTE FUNCTION public.log_dml_operations();

CREATE TRIGGER trg_log_issuance
    AFTER INSERT OR UPDATE OR DELETE ON public.issuance
    FOR EACH ROW EXECUTE FUNCTION public.log_dml_operations();

CREATE TRIGGER trg_log_booking
    AFTER INSERT OR UPDATE OR DELETE ON public.booking
    FOR EACH ROW EXECUTE FUNCTION public.log_dml_operations();
*/


INSERT INTO public.author (last_name, first_name) 
VALUES 
    ('Толстой', 'Лев'),
    ('Гоголь', 'Николай'),
    ('Пушкин', 'Александр'),
    ('Достоевский', 'Федор'),
    ('Чехов', 'Антон')
RETURNING id;

INSERT INTO public.publishing_house (house_name, city)
VALUES 
    ('Эксмо', 'Москва'),
    ('АСТ', 'Москва'),
    ('Просвещение', 'Санкт-Петербург'),
    ('Дрофа', 'Москва')
RETURNING id;

INSERT INTO public.book (book_name, author, house, "publication", publication_year, publication_count)
VALUES
    ('Война и мир', 1, 1, 1, 2010, 5000),
    ('Мертвые души', 2, 2, 1, 2015, 3000),
    ('Евгений Онегин', 3, 1, 1, 2018, 4000),
    ('Преступление и наказание', 4, 3, 1, 2012, 3500),
    ('Вишневый сад', 5, 4, 1, 2019, 2500)
RETURNING id;

INSERT INTO public.reader (reader_card, last_name, first_name, birth_date, gender, registration_date)
VALUES 
    (1001, 'Иванов', 'Иван', '1990-05-15', 'M', DEFAULT),
    (1002, 'Петрова', 'Мария', '1985-08-22', 'F', '2024-01-10'),
    (1003, 'Сидоров', 'Алексей', '1995-12-03', 'M', DEFAULT),
    (1004, 'Козлова', 'Анна', '1992-03-18', 'F', '2024-02-15')
RETURNING reader_card;

INSERT INTO public.book_instance (id, book_info, book_state, book_status, book_location)
VALUES 
    (5001, 1, 'best', 'available', 'A1-B2-C3'),
    (5002, 1, 'good', 'available', 'A1-B2-C4'),
    (5003, 2, 'old', 'available', 'B3-C1-D2'),
    (5004, 3, 'best', 'available', 'C2-D1-E3'),
    (5005, 4, 'good', 'available', 'D4-E2-F1'),
    (5006, 5, 'old', 'available', 'E3-F4-G2'),
    (5007, 1, 'best', 'available', 'A1-B2-C3')
RETURNING id;



-- 1.1 Add author
INSERT INTO public.author (last_name, first_name) 
VALUES 
    ('Тургенев', 'Иван')
RETURNING id;

-- 1.2 Update author
UPDATE public.author 
SET last_name = 'Лермонтов', first_name = 'Михаил' 
WHERE id = 6;

-- 1.3 Delete from author
DELETE FROM public.author 
WHERE id = 6;



-- 2.1 Insert into publishing house
INSERT INTO public.publishing_house (house_name, city)
VALUES 
    ('Азбука', 'Москва')
RETURNING id;

-- 2.2 Update publishing_house
UPDATE public.publishing_house
SET house_name = 'Бомбора', city = 'Вологда'
WHERE id = 5;

-- 2.3 Delete from publishing house
DELETE FROM public.publishing_house
WHERE id = 5;



-- 3.1 Insert into book
INSERT INTO public.book (book_name, author, house, "publication", publication_year, publication_count)
VALUES
    ('Герой нашего времени', 1, 1, 2, 2020, 2000)
RETURNING id;

-- 3.2 Update book
UPDATE public.book
SET book_name = 'Человек в футляре',
    author = 5,
    house = 4,
    "publication" = 1,
    publication_year = 2021,
    publication_count = 1500
WHERE id = 6;

-- 3.3 Delete from book
DELETE FROM public.book
WHERE id = 6;



-- 4.1 Insert readers
INSERT INTO public.reader (reader_card, last_name, first_name, birth_date, gender, registration_date)
VALUES 
    (1005, 'Смирнов', 'Петр', '1988-07-30', 'M', DEFAULT)
RETURNING reader_card;

-- 4.2 Update reader
UPDATE public.reader
SET 
    last_name = 'НеСмирнов',
    first_name = 'НеПётр',
    birth_date = '1990-05-20'
WHERE reader_card = 1005;

-- 4.3 Delete from reader
DELETE FROM public.reader
WHERE reader_card = 1005;




-- 5.1 Insert book instances
INSERT INTO public.book_instance (id, book_info, book_state, book_status, book_location)
VALUES 
    (5008, 2, 'good', 'available', 'F5-G1-H2')
RETURNING id;

-- 5.2 Update book instance
UPDATE public.book_instance
SET 
    book_state = 'good',
    book_status = 'reserved',
    book_location = 'A2-B1-C5'
WHERE id = 5008;

-- 5.3 Delete from book instance
DELETE FROM public.book_instance
WHERE id = 5008;



-- 6 Issue book to a reader
INSERT INTO public.issuance (reader_card, book_instance_id, issue_datetime, expected_return_date, actual_return_date)
VALUES 
    (1001, 5001, CURRENT_TIMESTAMP(0), CURRENT_DATE + INTERVAL '14 days', NULL)
RETURNING reader_card, book_instance_id;

UPDATE public.book_instance
SET book_status = 'issued'
WHERE id = 5001;



-- 7 Returning book
UPDATE public.issuance 
SET actual_return_date = CURRENT_DATE
WHERE reader_card = 1001 
  AND book_instance_id = 5001 
  AND actual_return_date IS NULL;

UPDATE public.book_instance
SET book_status = 'available'
WHERE id = 5001;



-- Create some issue
INSERT INTO public.issuance (reader_card, book_instance_id, issue_datetime, expected_return_date, actual_return_date)
VALUES 
    (1001, 5002, CURRENT_TIMESTAMP(0) - INTERVAL '20 days', CURRENT_DATE - INTERVAL '6 days', NULL),
    (1002, 5003, CURRENT_TIMESTAMP(0) - INTERVAL '10 days', CURRENT_DATE + INTERVAL '4 days', NULL),
    (1003, 5004, CURRENT_TIMESTAMP(0) - INTERVAL '25 days', CURRENT_DATE - INTERVAL '11 days', NULL);

UPDATE public.book_instance
SET book_status = 'issued'
WHERE id IN (5002, 5003, 5004);




-- 8 Create view for issued book instances
CREATE OR REPLACE VIEW public.issued_books_view AS
SELECT 
    r.last_name AS reader_last_name,
    r.first_name AS reader_first_name,
    a.last_name AS author_last_name,
    a.first_name AS author_first_name,
    b.book_name AS book_title,
    bi.book_state AS book_condition,
    i.issue_datetime AS issue_date
FROM public.issuance i
INNER JOIN public.reader r ON i.reader_card = r.reader_card
INNER JOIN public.book_instance bi ON i.book_instance_id = bi.id
INNER JOIN public.book b ON bi.book_info = b.id
INNER JOIN public.author a ON b.author = a.id
WHERE i.actual_return_date IS NULL;

SELECT * FROM public.issued_books_view;



-- 9 Create view for overdue issued books
CREATE OR REPLACE VIEW public.overdue_books_view AS
SELECT 
    r.last_name AS reader_last_name,
    r.first_name AS reader_first_name,
    a.last_name AS author_last_name,
    a.first_name AS author_first_name,
    b.book_name AS book_title,
    (CURRENT_DATE - i.expected_return_date) AS overdue_days
FROM public.issuance i
JOIN public.reader r ON i.reader_card = r.reader_card
JOIN public.book_instance bi ON i.book_instance_id = bi.id
JOIN public.book b ON bi.book_info = b.id
JOIN public.author a ON b.author = a.id
WHERE i.actual_return_date IS NULL 
  AND i.expected_return_date < CURRENT_DATE;

SELECT * FROM public.overdue_books_view;



-- 10 Issue book instance with overdue check
WITH overdue_check AS (
    SELECT COUNT(*) as overdue_count 
    FROM public.issuance i
    JOIN public.reader r ON i.reader_card = r.reader_card
    WHERE r.reader_card = 1004
      AND i.actual_return_date IS NULL 
      AND i.expected_return_date < CURRENT_DATE
)
INSERT INTO public.issuance (reader_card, book_instance_id, issue_datetime, expected_return_date, actual_return_date)
SELECT
	1004 as reader_card,
    5005 as book_instance_id,
    CURRENT_TIMESTAMP(0) as issue_datetime,
    CURRENT_DATE + INTERVAL '14 days' as expected_return_date,
    NULL as actual_return_date
FROM overdue_check
WHERE overdue_count = 0; 

-- Update book instance status only if issuance was successful
UPDATE public.book_instance
SET book_status = 'issued'
WHERE id = 5005
AND EXISTS (
    SELECT 1 FROM public.issuance 
    WHERE reader_card = 1004
    AND book_instance_id = 5005 
    AND actual_return_date IS NULL
);



-- 11 Book reservation with availability check
INSERT INTO public.booking (reader_card, book_id, min_condition, booking_datetime)
SELECT 1001, 1, 'best', CURRENT_TIMESTAMP(0)
WHERE EXISTS (
    SELECT 1 FROM public.book_instance 
    WHERE book_info = 1 
    AND book_state >= 'best' 
    AND book_status = 'available'
);

UPDATE public.book_instance 
SET book_status = 'reserved' 
WHERE book_info = 1 
AND book_state >= 'best' 
AND book_status = 'available';



-- 12 Cansel resevation
DELETE FROM public.booking 
WHERE reader_card = 1001 AND book_id = 1;

UPDATE public.book_instance 
SET book_status = 'available' 
WHERE book_info = 1 
AND book_status = 'reserved';



-- 13 Issue book instance with booking check
WITH 
overdue_check AS (
    SELECT COUNT(*) as overdue_count 
    FROM public.issuance i
    JOIN public.reader r ON i.reader_card = r.reader_card
    WHERE r.reader_card = 1004
      AND i.actual_return_date IS NULL 
      AND i.expected_return_date < CURRENT_DATE
),
booking_check AS (
    SELECT COUNT(*) as active_booking_count
    FROM public.booking b
    JOIN public.book_instance bi ON b.book_id = bi.book_info
    WHERE bi.id = 5007
      AND b.reader_card != 1004
      AND b.booking_datetime >= CURRENT_TIMESTAMP - INTERVAL '3 days'
)
INSERT INTO public.issuance (reader_card, book_instance_id, issue_datetime, expected_return_date, actual_return_date)
SELECT
    1004 as reader_card,
    5007 as book_instance_id,
    CURRENT_TIMESTAMP(0) as issue_datetime,
    CURRENT_DATE + INTERVAL '14 days' as expected_return_date,
    NULL as actual_return_date
FROM overdue_check, booking_check
WHERE overdue_check.overdue_count = 0 
  AND booking_check.active_booking_count = 0;

-- Update book instance status only if issuance was successful
UPDATE public.book_instance
SET book_status = 'issued'
WHERE id = 5007
AND EXISTS (
    SELECT 1 FROM public.issuance 
    WHERE reader_card = 1004
    AND book_instance_id = 5007
    AND actual_return_date IS NULL
);


-- 14 All books instance location
CREATE OR REPLACE FUNCTION get_book_locations(
    p_book_id INTEGER DEFAULT NULL,
    p_book_name VARCHAR DEFAULT NULL  
)
RETURNS TABLE (
    book_id INTEGER,
    book_name VARCHAR,                
    author_last_name VARCHAR,           
    author_first_name VARCHAR,        
    publishing_house VARCHAR,         
    publication_year INTEGER,
    instance_id INTEGER,
    book_state book_state_type,               
    book_status book_status_type,              
    book_location VARCHAR,
    state_priority INTEGER
) AS $$
BEGIN

    IF p_book_id IS NULL AND p_book_name IS NULL THEN
        RAISE EXCEPTION 'Необходимо указать ID книги или название книги';
    END IF;
    
    RETURN QUERY
    SELECT 
        b.id AS book_id,
        b.book_name,
        a.last_name AS author_last_name,
    	a.first_name AS author_first_name,
        ph.house_name AS publishing_house,
        b.publication_year,
        bi.id AS instance_id,
        bi.book_state,
        bi.book_status,
        bi.book_location,
        CASE 
            WHEN bi.book_state = 'best' 	THEN 1
            WHEN bi.book_state = 'good' 	THEN 2
            WHEN bi.book_state = 'normal' 	THEN 3
            WHEN bi.book_state = 'old' 		THEN 4
            ELSE 5
        END AS state_priority
    FROM public.book b
    INNER JOIN public.author a ON b.author = a.id
    INNER JOIN public.publishing_house ph ON b.house = ph.id
    INNER JOIN public.book_instance bi ON b.id = bi.book_info
    WHERE 
        (p_book_id IS NOT NULL AND b.id = p_book_id) OR
        (p_book_name IS NOT NULL AND b.book_name ILIKE '%' || p_book_name || '%')
    ORDER BY 
        state_priority ASC,
        bi.id ASC;
    
END;
$$ LANGUAGE plpgsql;

SELECT * FROM get_book_locations(p_book_id := 1);

SELECT * FROM get_book_locations(p_book_name := 'Война и мир');

/*SELECT book_name, author_last_name, author_first_name, COUNT(*) 
FROM get_book_locations(p_book_id := 1)
GROUP BY book_name, author_last_name, author_first_name;*/



-- 15 Create view for free books
CREATE OR REPLACE VIEW public.available_books_view AS
SELECT 
    b.id AS book_id,
    b.book_name,
    a.last_name AS author_last_name,
    a.first_name AS author_first_name,
    ph.house_name AS publishing_house,
    b.publication_year,
    bi.book_state,
    COUNT(bi.id) AS available_copies_count
FROM public.book b
INNER JOIN public.author a ON b.author = a.id
INNER JOIN public.publishing_house ph ON b.house = ph.id
INNER JOIN public.book_instance bi ON b.id = bi.book_info
WHERE bi.book_status = 'available'
GROUP BY 
    b.id, b.book_name, a.last_name, a.first_name, 
    ph.house_name, b.publication_year, bi.book_state
ORDER BY 
    b.book_name,
    CASE 
        WHEN bi.book_state = 'best' 	THEN 1
        WHEN bi.book_state = 'good' 	THEN 2
        WHEN bi.book_state = 'normal' 	THEN 3
        WHEN bi.book_state = 'old' 		THEN 4
        ELSE 5
    END;

SELECT * FROM public.available_books_view;



-- 16 Overdue more 1 year
CREATE OR REPLACE VIEW public.overdue_one_year_books_view AS
SELECT 
    r.reader_card,
    r.last_name AS reader_last_name,
    r.first_name AS reader_first_name,
    b.book_name,
    a.last_name AS author_last_name,
    a.first_name AS author_first_name,
    i.issue_datetime,
    i.expected_return_date,
    (CURRENT_DATE - i.issue_datetime::date) AS days_since_issue
FROM public.issuance i
INNER JOIN public.reader r ON i.reader_card = r.reader_card
INNER JOIN public.book_instance bi ON i.book_instance_id = bi.id
INNER JOIN public.book b ON bi.book_info = b.id
INNER JOIN public.author a ON b.author = a.id
WHERE i.actual_return_date IS NULL 
  AND i.issue_datetime < CURRENT_DATE - INTERVAL '1 year'
ORDER BY i.issue_datetime ASC;

SELECT * FROM public.overdue_one_year_books_view;