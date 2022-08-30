FROM node:16.14-alpine
WORKDIR /web

COPY package*.json ./

RUN npm install

COPY . .

# Build for production.
RUN npm run build --production

RUN npm install -g serve
