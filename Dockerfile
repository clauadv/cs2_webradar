# Build stage
FROM node:18-alpine AS build
WORKDIR /app
COPY webapp/package*.json ./webapp/
COPY .env ./webapp/.env
RUN cd webapp && npm install && npm run build

# Production stage
FROM node:18-alpine
WORKDIR /app
COPY --from=build /app/webapp/dist ./webapp/dist
COPY webapp/package*.json ./webapp/
COPY webapp/server.js ./webapp/server.js
RUN cd webapp && npm install --production
ENV NODE_ENV=production
CMD ["node", "webapp/server.js"]
