<h1 align="center">bangumi-status-card</h1>

<div align="center"><a href="https://wakatime.com/badge/user/e4fc89b9-21c5-4e72-b622-92abc1d68721/project/f09309c6-ae78-44fb-81a8-911d144e87e0"><img src="https://wakatime.com/badge/user/e4fc89b9-21c5-4e72-b622-92abc1d68721/project/f09309c6-ae78-44fb-81a8-911d144e87e0.svg" alt="wakatime" /></a></div>

---

## Feature

Display bangumi status in a form of a svg card, which can be attached to any kind of your personal introduction, such as blog, github profile and so on.

> [!NOTE]
> Unfortunately, since svg disallow remote image loading, there is currently no way to involve covers of anime in the github related cards. QAQ.
> Encoding the covers into base64 and embeding it into svg is a way, but I'm not with it because it's kind of complicated and performance risky.

## How to use

For markdown:

```markdown
![](bangumi-status-card.dvdbr3o.top/details?username=dvdbr3o)
```

For html:

```html
<img src="bangumi-status-card.dvdbr3o.top/details?username=dvdbr3o"></img>
```

### Examples

```markdown
![](http://bangumi-status-card.dvdbr3o.top/details?username=dvdbr3o&title=%E2%9C%85I%27ve%20watched&width=400)
```

![](http://bangumi-status-card.dvdbr3o.top/details?username=dvdbr3o&title=%E2%9C%85I%27ve%20watched&width=400)

```markdown
![](http://bangumi-status-card.dvdbr3o.top/details?username=dvdbr3o&collection_type=3&title=%F0%9F%91%80I%27m%20watching&width=400)
```

![](http://bangumi-status-card.dvdbr3o.top/details?username=dvdbr3o&collection_type=3&title=%F0%9F%91%80I%27m%20watching&width=400)

### Parameters

| Parameter       | Type    | Default | Description                                                                                     |
| --------------- | ------- | ------- | ----------------------------------------------------------------------------------------------- |
| username        | string  |         | The name of user.                                                                               |
| top             | number? | 0       | The number of anime listed. (not implemented)<br />0 for infinity                               |
| collection_type | enum?   | 2       | 1: Want to watch<br /> 2: Watched <br /> 3: On watching <br /> 4: Delayed <br /> 5: Deserted    |
| title           | string? | ""      | The title above.                                                                                |
| chinese         | bool?   | false   | Whether to use Chinese for bangumi name.                                                        |
| width           | number? | 800     | The width of card.                                                                              |
| view_box_width  | number? | 800     | The width of viewBox. The larger it is, the more displayed are the excessively long anime name. |

These parameters are for interface `details`. Later we will support another type `overview`.

## Deploy

### Docker

Bangumi status card can be self-hosted by docker.

```bash
docker pull dvdbrooo/bangumi-status-card
docker run -d -p 8080:8080 dvdbrooo/bangumi-status-card
```

## Related Projects

- [anuraghazra/github-readme-stats](https://github.com/anuraghazra/github-readme-stats): Status card about github user info.
- [open-ani/animeko](https://github.com/open-ani/animeko): A cross-platform and modern bangumi app that is highly recommended, anime status actions on which will be synchroized to bangumi, compatible with this repo.
- [bangumi/api](https://github.com/bangumi/api): The api of bangumi.
