CREATE TABLE contest_info (
    id SERIAL PRIMARY KEY,
    name VARCHAR(255) NOT NULL,
    contest_date DATE NOT NULL
);

CREATE TABLE participant (
    id SERIAL PRIMARY KEY,
    last_name VARCHAR(100) NOT NULL,
    first_name VARCHAR(100) NOT NULL,
    birth_date DATE NOT NULL,
    gender VARCHAR(10) CHECK (gender IN ('Male', 'Female'))
);

CREATE TABLE task (
    contest_id INTEGER NOT NULL,
    task_id INTEGER NOT NULL,
    task_name VARCHAR(255) NOT NULL,
    description TEXT,
    PRIMARY KEY (contest_id, task_id),
    FOREIGN KEY (contest_id) REFERENCES contest_info(id) ON DELETE CASCADE
);

CREATE TABLE solution (
    task_id INTEGER NOT NULL,
    contest_id INTEGER NOT NULL,
    participant_id INTEGER NOT NULL,
    solution_text TEXT NOT NULL,
    programming_language VARCHAR(50) NOT NULL,
    language_version VARCHAR(20),
    score INTEGER CHECK (score >= 0 AND score <= 100),
    reviewer_comment TEXT,
    PRIMARY KEY (task_id, contest_id, participant_id),
    FOREIGN KEY (contest_id, task_id) REFERENCES task(contest_id, task_id) ON DELETE CASCADE,
    FOREIGN KEY (participant_id) REFERENCES participant(id) ON DELETE CASCADE
);

-- Заполнение данными

-- Конкурсы
INSERT INTO contest_info (name, contest_date) VALUES
('Winter Programming Championship 2024', '2024-02-15'),
('Spring Algorithmic Battles', '2024-04-10'),
('Summer AI Solutions Hackathon', '2024-07-22');

-- Участники
INSERT INTO participant (last_name, first_name, birth_date, gender) VALUES
('Ivanov', 'Alexey', '2000-03-15', 'Male'),
('Petrova', 'Maria', '1999-11-22', 'Female'),
('Sidorov', 'Dmitry', '2001-05-08', 'Male'),
('Kozlova', 'Anna', '2000-08-30', 'Female'),
('Vasiliev', 'Ivan', '1998-12-10', 'Male');

-- Задачи
INSERT INTO task (contest_id, task_id, task_name, description) VALUES
(1, 1, 'Sum of Two Numbers', 'Write a function that returns the sum of two integers'),
(1, 2, 'Palindrome', 'Determine if a string is a palindrome'),
(1, 3, 'Array Sorting', 'Implement the quicksort algorithm'),
(2, 1, 'Depth-First Search', 'Implement graph depth-first traversal algorithm'),
(2, 2, 'Dynamic Programming', '0-1 Knapsack problem'),
(3, 1, 'Image Classification', 'Create a model for handwritten digit classification'),
(3, 2, 'Natural Language Processing', 'Text generation using RNN');

-- Решения
INSERT INTO solution (contest_id, task_id, participant_id, solution_text, programming_language, language_version, score, reviewer_comment) VALUES
(1, 1, 1, 'def sum(a, b): return a + b', 'Python', '3.9', 95, 'Excellent solution'),
(1, 1, 2, 'function sum(a, b) { return a + b; }', 'JavaScript', 'ES6', 90, 'Good, but error handling could be improved'),
(1, 2, 1, 'def is_palindrome(s): return s == s[::-1]', 'Python', '3.9', 88, 'Efficient but not memory optimal'),
(1, 2, 3, 'bool isPalindrome(string s) { /* C++ implementation */ }', 'C++', '17', 92, 'Good implementation'),
(1, 3, 1, 'def quicksort(arr): /* implementation */', 'Python', '3.9', 85, 'Performance issues with large data'),
(1, 3, 4, 'public class QuickSort { /* Java implementation */ }', 'Java', '11', 89, 'Correct but could be optimized'),
(2, 1, 2, '// DFS implementation in Python', 'Python', '3.9', 91, 'Correct algorithm'),
(2, 2, 3, '// Knapsack problem solution', 'C++', '17', 87, 'Non-optimal complexity'),
(3, 1, 1, '# CNN model for classification', 'Python', '3.9', 94, 'Excellent model architecture'),
(3, 1, 5, '// TensorFlow implementation', 'Python', '3.9', 89, 'Good but accuracy needs improvement'),
(3, 2, 4, '# RNN for text generation', 'Python', '3.9', 86, 'Interesting approach but training issues');