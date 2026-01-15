DROP DATABASE IF EXISTS t01_library;

CREATE DATABASE t01_library;

\c t01_library

CREATE TYPE book_state_type AS ENUM ('best', 'good', 'normal', 'old', 'lost');
CREATE TYPE book_status_type AS ENUM ('available', 'issued', 'reserved');

CREATE TABLE public.author (
    id SERIAL PRIMARY KEY,
    last_name VARCHAR(100) NOT NULL,
    first_name VARCHAR(100) NOT NULL
);

CREATE TABLE public.publishing_house (
    id SERIAL PRIMARY KEY,
    house_name VARCHAR(200) NOT NULL,
    city VARCHAR(100) NOT NULL
);

CREATE TABLE public.book (
    id SERIAL PRIMARY KEY,
    book_name VARCHAR(100) NOT NULL,
    author INT REFERENCES public.author(id),
    house INT REFERENCES public.publishing_house(id),
    publication INT NOT NULL,
    publication_year INT,
    publication_count INT
);

CREATE TABLE public.reader (
    reader_card INT PRIMARY KEY,
    last_name VARCHAR(100) NOT NULL,
    first_name VARCHAR(100) NOT NULL,
    birth_date DATE NOT NULL,
    gender CHAR(1) NOT NULL,
    registration_date DATE NOT NULL DEFAULT CURRENT_DATE
);

CREATE TABLE public.book_instance (
    id INT PRIMARY KEY,
    book_info INT REFERENCES public.book(id),
    book_state book_state_type NOT NULL DEFAULT 'good',
    book_status book_status_type NOT NULL DEFAULT 'available',
    book_location VARCHAR(10) NOT NULL
);

CREATE TABLE public.issuance (
    reader_card INT REFERENCES public.reader(reader_card),
    book_instance_id INT REFERENCES public.book_instance(id),
    issue_datetime TIMESTAMP(0) NOT NULL DEFAULT CURRENT_TIMESTAMP(0),
    expected_return_date DATE NOT NULL,
    actual_return_date DATE NULL,
    
    PRIMARY KEY (reader_card, book_instance_id)
);

CREATE TABLE public.booking (
    booking_id SERIAL PRIMARY KEY,
    reader_card INT REFERENCES public.reader(reader_card),
    book_id INTEGER NOT NULL REFERENCES public.book(id),
    min_condition book_state_type NOT NULL DEFAULT 'normal',
    booking_datetime TIMESTAMP(2) NOT NULL DEFAULT CURRENT_TIMESTAMP(0)
);

CREATE TABLE public.logs (
    log_id SERIAL PRIMARY KEY,
    log_datetime TIMESTAMP(0) NOT NULL DEFAULT CURRENT_TIMESTAMP(0),
    log_context VARCHAR(50) NOT NULL,
    log_content TEXT NOT NULL
);