# SQL homework - Algorithms and Data Structures

Start docker container with clickhouse image:
```bash
docker run -d --name clickhouse -p 9000:9000 -e CLICKHOUSE_USER=user -e CLICKHOUSE_PASSWORD=password -e CLICKHOUSE_DB=test clickhouse/clickhouse-server
```
Start docker container with postgres image:
```bash
docker run --name postgres -e POSTGRES_PASSWORD=pass -p 5432:5432 -d postgres
```

if problems with postgres, run:
```bash
brew install posgresql
brew install libpq
brew link --force libpq
```