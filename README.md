# webserver
Let's create a simple server using C++
Ok lets do it :)!
***

### Docs

[HTTP Semantics RFC](https://www.rfc-editor.org/rfc/rfc9110.pdf)

[ABNF RFC](https://www.rfc-editor.org/rfc/pdfrfc/rfc5234.txt.pdf)

[MIME RFC](https://www.rfc-editor.org/rfc/pdfrfc/rfc2045.txt.pdf)

[URI RFC](https://www.rfc-editor.org/rfc/pdfrfc/rfc3986.txt.pdf)

[HTTP/1.1 RFC](https://www.rfc-editor.org/rfc/rfc9112.pdf)

[Simple-webserver-from-scratch](https://medium.com/from-the-scratch/http-server-what-do-you-need-to-know-to-build-a-simple-http-server-from-scratch-d1ef8945e4fa)

[NGINX webserver configuration file tutorial](https://www.plesk.com/blog/various/nginx-configuration-guide/)

### Guide

- HTML / POST / DELETE

[https://www.youtube.com/watch?v=nMGlaiNBbNU&list=PL6n9fhu94yhW7yoUOGNOfHurUE6bpOO2b&index=4](https://www.youtube.com/watch?v=nMGlaiNBbNU&list=PL6n9fhu94yhW7yoUOGNOfHurUE6bpOO2b&index=4)

[https://www.youtube.com/watch?v=eesqK59rhGA](https://www.youtube.com/watch?v=eesqK59rhGA)

- MIME

[https://www.youtube.com/watch?v=FBkZ2TJZZUY](https://www.youtube.com/watch?v=FBkZ2TJZZUY)

Good Things to Read related with epool:
- documentation

[https://copyconstruct.medium.com/the-method-to-epolls-madness-d9d2d6378642](https://copyconstruct.medium.com/the-method-to-epolls-madness-d9d2d6378642)

[https://www.youtube.com/watch?v=O-yMs3T0APU](https://www.youtube.com/watch?v=O-yMs3T0APU)

[https://www.youtube.com/watch?v=dEHZb9JsmOU](https://www.youtube.com/watch?v=dEHZb9JsmOU)

<details>
	<summary><h3>Directives</h3></summary>

| Directive | Description | Example |
| --------- | ----------- | ------- |
| auth | restrict route to a user | auth user:route |
| autoindex | directory listing on & off | autoindex off \| on  |
| cgi | cgi | cgi |
| cgi-bin | cgi-bin | cgi-bin |
| client_max_body_size | Limit client body size | client_max_body_size #bytes|
| error_page | Setup default error pages | error_page code1 [code2] ... path |
| limit_methods | Define a list of accepted HTTP methods for the route (inside location scope) | limit_methods METHOD |
| listen | Choose the port and host of each ’server' | listen host:port |
| location | Setup routes with one or multiple rules/configuration | location route { ... }|
| root | root | root |
| server_name | Setup the server_names or not | server_name name1 [name2] ...|
| upload | upload | upload |
</details>
