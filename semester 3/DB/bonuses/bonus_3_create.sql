CREATE DATABASE bonus_3;

CREATE TABLE IF NOT EXISTS public.service
(
    id SERIAL NOT NULL PRIMARY KEY,
    name VARCHAR(150) NOT NULL
);

CREATE TABLE IF NOT EXISTS public.subscription
(
    id SERIAL NOT NULL PRIMARY KEY,
    name VARCHAR(150) NOT NULL,
    price MONEY NOT NULL,
    description TEXT NOT NULL
);

CREATE TABLE IF NOT EXISTS public.client
(
    id SERIAL NOT NULL PRIMARY KEY,
    name VARCHAR(150) NOT NULL,
    surname VARCHAR(150) NOT NULL,
    sex BOOL NOT NULL,
    birthday DATE NOT NULL,
    subscription_id INT NOT NULL,
    subscription_start DATE NOT NULL,
    subscription_end DATE NOT NULL,
    FOREIGN KEY(subscription_id) REFERENCES public.subscription(id)
);

CREATE TABLE IF NOT EXISTS public.sub_to_service
(
    sub_id INT NOT NULL,
    service_id INT NOT NULL,
    PRIMARY KEY (sub_id, service_id),
    FOREIGN KEY(sub_id) REFERENCES public.subscription(id),
    FOREIGN KEY(service_id) REFERENCES public.service(id)
);