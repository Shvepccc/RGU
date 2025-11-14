--1
SELECT DISTINCT 
	p.id,
	p.first_name,
	p.last_name 
FROM participant p 
JOIN solution s ON p.id = s.participant_id
JOIN contest_info ci ON s.contest_id = ci.id
WHERE
	ci.contest_date = '2025-11-03'
	AND p.gender = 'Male'
	AND birth_date <= '2007-11-03';


--2
SELECT * FROM task t 
WHERE
	t.description ILIKE '%variable%'
	OR t.description  ILIKE '%value%';

--3
SELECT 
    p.last_name,
    p.first_name,
    EXTRACT(YEAR FROM AGE(CURRENT_DATE, p.birth_date)) as age
FROM participant p 
WHERE 
	p.id IN (
	    SELECT s.participant_id 
	    FROM solution s
	    GROUP BY s.participant_id
	    HAVING COUNT(DISTINCT s.contest_id) >= 2);

--4
SELECT DISTINCT t.task_id FROM solution s
JOIN task t ON s.task_id = t.task_id
WHERE
	s.score = 100;

--5
SELECT DISTINCT t.task_id FROM solution s
JOIN task t ON s.task_id = t.task_id
WHERE
	s.score < 52
	AND s.programming_language = 'Python';

--6
SELECT name FROM contest_info
WHERE EXTRACT(MONTH FROM contest_date) IN (6, 7, 8);

--7
SELECT 
    programming_language,
    language_version,
    COUNT(*) as tasks_solved
FROM solution
WHERE score >= 85
GROUP BY programming_language, language_version
HAVING COUNT(*) >= 2;

--8
SELECT 
    t.contest_id,
    t.task_id,
    t.task_name,
    s.solution_text
FROM task t
JOIN solution s ON t.contest_id = s.contest_id AND t.task_id = s.task_id
WHERE s.programming_language = 'C'
    AND s.language_version = 'C99'
    AND s.solution_text LIKE '%#include <string.h>%';

--9
SELECT 
    s.contest_id,
    c.name as contest_name,
    s.programming_language,
    s.language_version,
    COUNT(DISTINCT s.participant_id) as participant_count
FROM solution s
JOIN contest_info c ON s.contest_id = c.id
WHERE (s.contest_id, s.participant_id) IN (
    SELECT s1.contest_id, s1.participant_id
    FROM solution s1
    GROUP BY s1.contest_id, s1.participant_id, s1.programming_language, s1.language_version
    HAVING COUNT(DISTINCT s1.task_id) = (
        SELECT COUNT(DISTINCT t.task_id)
        FROM task t
        WHERE t.contest_id = s1.contest_id
    )
)
AND (s.contest_id, s.participant_id) IN (
    SELECT s2.contest_id, s2.participant_id
    FROM solution s2
    GROUP BY s2.contest_id, s2.participant_id
    HAVING AVG(s2.score) > 75 AND MIN(s2.score) >= 50
)
GROUP BY s.contest_id, c.name, s.programming_language, s.language_version
HAVING COUNT(DISTINCT s.participant_id) >= 3;

--10
SELECT DISTINCT s.programming_language 
FROM solution s 
JOIN contest_info ci ON ci.id = s.contest_id 
WHERE ci."name" = 'Summer AI Solutions Hackathon';

--11
SELECT 
    p.id,
    p.last_name,
    p.first_name
FROM participant p
WHERE 
	p.id IN (
	    SELECT s.participant_id
	    FROM solution s
	    JOIN contest_info c ON s.contest_id = c.id
	    WHERE c.name = 'памагити((9(9'
	    GROUP BY s.participant_id, c.id
	    HAVING 
	    	MIN(s.score) >= 45 
	        AND COUNT(DISTINCT s.task_id) = (
	            SELECT COUNT(*) 
	            FROM task t 
	            WHERE t.contest_id = c.id
	        )
	)
	AND p.id IN (
	    SELECT s2.participant_id
	    FROM solution s2
	    JOIN contest_info c2 ON s2.contest_id = c2.id
	    WHERE c2.name = 'памагити((9(9'
	    GROUP BY s2.participant_id
	    HAVING COUNT(DISTINCT s2.task_id) = COUNT(DISTINCT s2.programming_language || '|' || s2.language_version)
	);

--12
SELECT 
    p.id,
    p.last_name,
    p.first_name
FROM participant p
WHERE p.gender = 'Female'
AND p.id IN (
    SELECT s.participant_id
    FROM solution s
    WHERE s.score >= 52
    GROUP BY s.participant_id, s.contest_id
    HAVING COUNT(DISTINCT s.task_id) >= 3
);

--13
SELECT 
    p.id,
    p.last_name,
    p.first_name
FROM participant p
WHERE p.last_name = 'Демченко'
AND p.id IN (
    SELECT s.participant_id
    FROM solution s
    JOIN contest_info c ON s.contest_id = c.id
    JOIN (
        SELECT contest_id, COUNT(*) as task_count
        FROM task
        GROUP BY contest_id
    ) ct ON s.contest_id = ct.contest_id
    WHERE 
    	c.name = 'no way back' 
        AND c.contest_date = '2025-11-03'
        AND s.score = 0
    GROUP BY s.participant_id, ct.task_count
    HAVING COUNT(DISTINCT s.task_id) = ct.task_count
)
AND p.id IN (
    -- Участники, которые все задачи контеста написали на C++
    SELECT s.participant_id
    FROM solution s
    JOIN contest_info c ON s.contest_id = c.id
    WHERE 
    	c.name = 'no way back' 
        AND c.contest_date = '2025-11-03'
    GROUP BY s.participant_id
    HAVING COUNT(*) = SUM(CASE WHEN s.programming_language = 'C++' THEN 1 ELSE 0 END)
)
AND p.id IN (
    SELECT s.participant_id
    FROM solution s
    JOIN contest_info c ON s.contest_id = c.id
    WHERE c.name = 'no way back' 
        AND c.contest_date = '2025-11-03'
        AND s.solution_text LIKE '%unsigned double%'
);

--14
SELECT
    t.task_id,
    t.task_name,
    AVG(s.score) AS average_score
FROM solution s 
JOIN task t ON s.task_id = t.task_id 
WHERE s.score >= 70
GROUP BY t.task_id, t.task_name 
HAVING AVG(s.score) BETWEEN 80 AND 85;

--15
SELECT DISTINCT
    t.task_id,
    t.task_name,
    t.description
FROM task t
JOIN solution s ON t.task_id = s.task_id AND t.contest_id = s.contest_id
WHERE s.reviewer_comment ILIKE '%где лабы?%';