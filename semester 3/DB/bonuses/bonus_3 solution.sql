-- 1
SELECT DISTINCT c.surname
FROM public.client c
JOIN public.subscription s ON c.subscription_id = s.id
WHERE s.name = 'demo_sub';

-- 2
SELECT *
FROM public.client c
WHERE EXTRACT(YEAR FROM c.subscription_end) = EXTRACT(YEAR FROM CURRENT_DATE);

-- 3
SELECT s.id, s.name, s.price, s.description, COUNT(c.id) AS clients_count
FROM public.subscription s
JOIN public.client c ON c.subscription_id = s.id
GROUP BY 
	s.id, s.name, s.price, s.description
HAVING 
	COUNT(c.id) >= 10;

-- 4
SELECT s.id, s.name, COUNT(DISTINCT sts.service_id) AS services
FROM public.subscription s
JOIN public.sub_to_service sts ON s.id = sts.sub_id
GROUP BY
	s.id, s.name
HAVING
	COUNT(DISTINCT sts.service_id) > 5;

-- 5
SELECT c.surname, c.name
FROM public.client c
JOIN public.subscription s ON c.subscription_id = s.id
JOIN public.sub_to_service sts ON s.id = sts.sub_id
JOIN public.service serv ON sts.service_id = serv.id
WHERE 
	c.sex = TRUE
  	AND serv.name = 'отчислиться на АиСД';

-- 6
SELECT
	sub.id AS sub_id,
	sub.name AS sub_name,
	serv.id AS serv_id,
	serv.name AS serv_name
FROM public.subscription sub
JOIN public.sub_to_service sts ON sub.id = st.sub_id
JOIN public.service serv ON serv.id = sts.service_id
ORDER BY sub.id, serv.id;

-- 7
SELECT *
FROM public.client c
WHERE c.subscription_end < CURRENT_DATE;

-- 8
SELECT c.surname, c.name
FROM public.client c
JOIN public.subscription sub ON c.subscription_id = sub.id
WHERE
	sub.name = 'ПМИ'
  	AND c.subscription_end = DATE '2025-11-03';

-- 9
SELECT c.id, c.surname, c.name, s.id AS sub_id, s.name AS sub_name, s.price
FROM public.client c
JOIN public.subscription s ON s.id = c.subscription_id
WHERE s.price::numeric > 52000;

-- 10. Все женщины, у которых в подписке есть услуга с id = 3.
SELECT c.id, c.surname, c.name
FROM public.client c
JOIN public.subscription sub ON c.subscription_id = sub.id
JOIN public.sub_to_service sts ON s.id = sts.sub_id
WHERE 
	c.sex = FALSE
 	AND sts.service_id = 3;

-- 11
CREATE OR REPLACE VIEW public.active_services AS
SELECT DISTINCT se.id, se.name
FROM public.service se
WHERE EXISTS (
    SELECT 1
    FROM public.sub_to_service sts
	JOIN public.client c ON c.subscription_id = sts.sub_id
    WHERE sts.service_id = se.id
);

-- 12
UPDATE public.client 
SET 
    subscription_id = 3,
    subscription_start = CURRENT_DATE,
    subscription_end = CURRENT_DATE + INTERVAL '1 month'
WHERE id = 5;

-- 13
UPDATE public.client 
SET 
    subscription_id = (SELECT id FROM public.subscription WHERE name = 'Название устанавливаемой подписки'),
    subscription_start = CURRENT_DATE,
    subscription_end = CURRENT_DATE + INTERVAL '1 month'
WHERE id = 5;

-- 14
SELECT COUNT(DISTINCT c.id) AS men_count
FROM public.client c
JOIN public.subscription sub ON c.subscription_id = sub.id
JOIN public.sub_to_service sts ON su.id = sts.sub_id
JOIN public.service se ON sts.service_id = se.id
WHERE 
	(EXTRACT(YEAR FROM AGE(CURRENT_DATE, c.birthday))) > 35
  	AND se.name = 'Бонусы для 2 курса'
  	AND c.sex = TRUE;

-- 15
SELECT 
    sub.id,
    sub.name,
    sub.price,
    sub.description,
    COUNT(DISTINCT sts.service_id) AS services_count,
    COUNT(DISTINCT CASE WHEN c.sex = TRUE THEN c.id END) AS men_count,
    COUNT(DISTINCT CASE WHEN c.sex = FALSE THEN c.id END) AS women_count
FROM public.subscription sub
JOIN public.sub_to_service sts ON sub.id = sts.sub_id
JOIN public.client c ON sub.id = c.subscription_id
GROUP BY sub.id, sub.name, sub.price, sub.description
HAVING
    sub.price::numeric > 85000
    AND COUNT(DISTINCT sts.service_id) <= 10
    AND COUNT(DISTINCT CASE WHEN c.sex = TRUE THEN c.id END) >= 10
    AND COUNT(DISTINCT CASE WHEN c.sex = FALSE THEN c.id END) >= 10;
