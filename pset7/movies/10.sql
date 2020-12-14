-- In 10.sql, write a SQL query to list the names of all people who have directed a movie
-- that received a rating of at least 9.0.
--Your query should output a table with a single column for the name of each person.

SELECT name FROM people
WHERE id IN
(SELECT DISTINCT person_id
FROM directors
WHERE movie_id IN (
SELECT DISTINCT movie_id
FROM ratings
WHERE rating >= 9.0)
);