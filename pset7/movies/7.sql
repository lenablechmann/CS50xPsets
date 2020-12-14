SELECT title, rating
FROM "ratings"
JOIN "movies" ON ratings.movie_id=movies.id
WHERE year=2010
GROUP BY title
ORDER BY rating DESC;