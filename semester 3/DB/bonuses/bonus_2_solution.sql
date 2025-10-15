-- 1
SELECT s.first_name , s.last_name FROM student s 
INNER JOIN student_group sg ON s.group_id = sg.id
WHERE sg.course = 3;

-- 2
SELECT sg.id, count(*)
FROM student s 
INNER JOIN student_group sg ON s.group_id = sg.id
GROUP BY sg.id
HAVING count(*) > 25
ORDER BY sg.id ASC;

-- 3
SELECT 
    sb.name AS subject_name,
    AVG(m.grade) AS average_grade
FROM  subject sb
JOIN marks m ON sb.id = m.subj_id
JOIN reporting_type rt ON sb.reporting_type_id = rt.id
WHERE 
    m.semester_number = 4
    AND rt.type_name = 'exam'
GROUP BY sb.id, sb.name
HAVING AVG(m.grade) > 3.5
ORDER BY average_grade DESC;

-- 4
SELECT s.id, s.first_name, s.last_name
FROM student s 
JOIN marks m ON s.id = m.stud_id
JOIN subject sb ON m.subj_id = sb.id
WHERE
	m.semester_number = 3
	AND sb.reporting_type_id = 2
GROUP BY
	s.id
HAVING
	AVG(m.grade) = 5
LIMIT 10;

-- 5
SELECT DISTINCT sb.name
FROM subject sb
JOIN marks m ON m.subj_id  = sb.id
WHERE 
	sb.reporting_type_id = 3
	AND m.semester_number BETWEEN 3 AND 4;

-- 6
SELECT semester_number FROM (
    SELECT 
        m.semester_number, 
        s.id, 
        AVG(m.grade) as avg_grade
    FROM student s
    JOIN student_group sg ON s.group_id = sg.id
    JOIN marks m ON s.id = m.stud_id
    JOIN subject sb ON m.subj_id = sb.id
    WHERE 
        sg.code = 'GROUP-6'
        AND sb.reporting_type_id = 3
    GROUP BY
        m.semester_number, s.id
    HAVING
        AVG(m.grade) > 4.5
)
GROUP BY semester_number
HAVING COUNT(*) >= 3;

-- 7
SELECT s.id, s.first_name, s.birth_date 
FROM student s 
JOIN student_group sg ON s.group_id = sg.id
WHERE sg.course = 2 OR sg.course = 4
ORDER BY s.birth_date ASC
LIMIT 5;

-- 8
SELECT sb.name
FROM student s 
JOIN student_group sg ON s.group_id = sg.id 
JOIN marks m ON s.id = m.stud_id
JOIN subject sb ON m.subj_id = sb.id
WHERE
	sg.code = 'GROUP-6'
GROUP BY
	sb.name
HAVING 
	 AVG(m.grade) BETWEEN 3.0 AND 4.0
ORDER BY
	sb.name;

-- 9
SELECT s.last_name
FROM student s 
JOIN marks m ON s.id = m.stud_id
JOIN subject sb ON m.subj_id = sb.id
WHERE
    m.semester_number = 3
    AND sb.name = 'Subject_6'
    AND m.grade = 2;

-- 10
SELECT s.last_name
FROM student s 
JOIN marks m ON s.id = m.stud_id
JOIN subject sb ON m.subj_id = sb.id
WHERE
    m.semester_number = 4
    AND sb.name = 'Subject_11'
    AND m.grade != 2
    AND sb.reporting_type_id = 3;

-- 11
SELECT s.id, s.first_name, s.last_name
FROM student s 
JOIN marks m ON s.id = m.stud_id
JOIN subject sb ON m.subj_id = sb.id
WHERE
	m.grade = 2 
	AND sb.reporting_type_id = 2;

-- 12
SELECT sg.code
FROM student s 
JOIN marks m ON s.id = m.stud_id
JOIN subject sb ON m.subj_id = sb.id
JOIN student_group sg ON s.group_id = sg.id 
WHERE 
	m.grade = 2 
	AND sb.reporting_type_id = 3
GROUP BY sg.code;

-- 13
SELECT DISTINCT sb.name AS subject_name
FROM subject sb
JOIN marks m ON sb.id = m.subj_id
JOIN student s ON m.stud_id = s.id
JOIN student_group g ON s.group_id = g.id
WHERE 
    m.grade = 2
    AND s.id IN (
        SELECT stud_id
        FROM marks
        WHERE grade = 2
        GROUP BY stud_id
        HAVING COUNT(*) >= 2
    )
ORDER BY 
    sb.name;

--14
SELECT s.id, s.first_name, s.last_name, s.group_id, AVG(m.grade) AS avg_value
FROM student s 
JOIN marks m ON s.id = m.stud_id
JOIN subject sb ON m.subj_id = sb.id
JOIN student_group sg ON s.group_id = sg.id 
WHERE
	sg.code = 'GROUP-8'
	AND m.semester_number = 3
GROUP BY
	s.id
ORDER BY
	avg_value DESC;

-- 15
SELECT s.first_name, AVG(m.grade) AS avg_value
FROM student s 
JOIN marks m ON s.id = m.stud_id
JOIN subject sb ON m.subj_id = sb.id
WHERE
    sb.name = 'Subject_5'
    AND s.first_name LIKE 'FirstName_1_%'
GROUP BY
    s.first_name;

-- 16
SELECT 
	s.id,
	s.first_name,
	m.semester_number,
	AVG(m.grade) AS avg_value
FROM student s 
JOIN marks m ON s.id = m.stud_id
JOIN subject sb ON m.subj_id = sb.id
WHERE
	s.first_name LIKE 'FirstName%'
GROUP BY
	s.id, m.semester_number
ORDER BY
	s.id ASC, m.semester_number ASC;

-- 17
SELECT DISTINCT subject_name
FROM (
    SELECT 
        sb.id AS subject_id,
        sb.name AS subject_name,
        sg.id AS group_id,
        COUNT(CASE WHEN m.grade = 2 THEN 1 END) AS failed_count
    FROM student s
    JOIN marks m ON s.id = m.stud_id
    JOIN subject sb ON m.subj_id = sb.id
    JOIN student_group sg ON s.group_id = sg.id
    GROUP BY
        sb.id, sb.name, sg.id
) AS t1
JOIN (
    SELECT 
        sg.id AS group_id, 
        COUNT(*) AS total_students
    FROM student_group sg
    JOIN student s ON sg.id = s.group_id
    GROUP BY sg.id
) AS t2 
ON t1.group_id = t2.group_id
WHERE t1.failed_count >= t2.total_students * 0.5
ORDER BY subject_name ASC;

-- 18
SELECT sb."name" 
FROM student s
JOIN marks m ON s.id = m.stud_id
JOIN subject sb ON m.subj_id = sb.id
WHERE
	sb.reporting_type_id = 1
	AND sb.id IN (
		SELECT
			sb.id
		FROM marks m
		JOIN subject sb ON m.subj_id = sb.id
		WHERE m.grade = 2
		GROUP BY sb.id
		HAVING 
			COUNT(CASE WHEN m.grade = 2 THEN 1 END) > 5)
GROUP BY
	sb."name"
ORDER BY
	sb."name" ASC;

-- 19
SELECT 
	s.id,
	s.first_name,
	sb."name",
	AVG(m.grade) AS avg_value
FROM student s
JOIN marks m ON s.id = m.stud_id
JOIN subject sb ON m.subj_id = sb.id
WHERE
	s.id IN (
		SELECT s.id
		FROM student s
		JOIN marks m ON s.id = m.stud_id
		JOIN subject sb ON m.subj_id = sb.id
		GROUP BY s.id
		HAVING COUNT(CASE WHEN m.grade = 2 THEN 1 END) = 0)
GROUP BY s.id, sb."name";

-- 20
SELECT 
    s.id,
    s.first_name,
    s.last_name
FROM student s
JOIN marks m ON s.id = m.stud_id
GROUP BY s.id, s.first_name, s.last_name
HAVING 
    COUNT(CASE WHEN m.semester_number = 3 AND m.grade = 2 THEN 1 END) = 0
    AND COUNT(CASE WHEN m.semester_number IN (1, 2) AND m.grade = 2 THEN 1 END) > 0;

	




	
	

