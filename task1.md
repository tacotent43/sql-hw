# Задача 1.

## 1.1. Таблица Employees. Получить список всех сотрудников, у которых зарплата находится в промежутке от 8000 до 9000 (включительно) и/или кратна 1000

```sql
SELECT * 
FROM employees
WHERE salary BETWEEN 8000 AND 9000
   OR salary % 1000 = 0;
```

## 1.2. Таблица Employees. Получить первое трёхзначное число телефонного номера сотрудника, если его номер в формате ХХХ.ХХХ.ХХХХ

```sql
SELECT SUBSTR(phone_number, 1, 3)
FROM employees;
```

## 1.3. Таблица Departments. Получить первое слово из имени департамента для тех, у кого в названии больше одного слова

```sql
SELECT SUBSTR(department_name, 1, INSTR(department_name, ' ') - 1)
FROM departments
WHERE department_name LIKE '% %';
```

## 1.4. Таблица Employees. Получить список всех сотрудников, которые пришли на работу в первый день месяца (любого)

```sql
SELECT *
FROM employees
WHERE STRFTIME('%d', hire_date) = '01';
```

## 1.5. Таблица Employees. Получить уровень зарплаты каждого сотрудника: Меньше 5000 считается Low level, Больше или равно 5000 и меньше 10000 считается Normal level, Больше или равно 10000 считается High level

```sql
SELECT first_name,
CASE
    WHEN salary < 5000 THEN 'Low level'
    WHEN salary < 10000 THEN 'Normal level'
    ELSE 'High level'
END AS salary_level
FROM employees;
```

## 1.6. Таблица Employees. Получить список department_id и округленную среднюю зарплату работников в каждом департаменте.

```sql
SELECT department_id, ROUND(AVG(salary))
FROM employees
GROUP BY department_id;
```

## 1.7. Таблицы Employees, Departments, Locations, Countries, Regions. Получить список регионов и количество сотрудников в каждом регионе

```sql
SELECT r.region_name, COUNT(e.employee_id)
FROM employees e
JOIN departments d ON e.department_id = d.department_id
JOIN locations l ON d.location_id = l.location_id
JOIN countries c ON l.country_id = c.country_id
JOIN regions r ON c.region_id = r.region_id
GROUP BY r.region_name;
```

## 1.8. Таблица Employees. Показать всех менеджеров, которые имеют в подчинении больше шести сотрудников

```sql
SELECT manager_id, COUNT(*)
FROM employees
GROUP BY manager_id
HAVING COUNT(*) > 6;
```

## 1.9. Таблицы Employees, Departments. Показать все департаменты, в которых работают больше пяти сотрудников

```sql
SELECT d.department_name
FROM departments d
JOIN employees e ON d.department_id = e.department_id
GROUP BY d.department_id
HAVING COUNT(*) > 5;
```

## 1.10. Таблица Employees. Получить список сотрудников с зарплатой большей средней зарплаты всех сотрудников.

```sql
SELECT *
FROM employees
WHERE salary > (SELECT AVG(salary) FROM employees);
```