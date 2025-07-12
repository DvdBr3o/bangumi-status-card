<h1 align="center">bangumi-status-card</h1>

---

## Feature

Display bangumi status in a form of a svg card, which can be attached to any kind of your personal introduction, such as blog, github profile and so on.

> [!WARNING]
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

## Related Projects

- [anuraghazra/github-readme-stats](https://github.com/anuraghazra/github-readme-stats): Status card about github user info.
- [open-ani/animeko](https://github.com/open-ani/animeko): A cross-platform and modern bangumi app that is highly recommended, anime status actions on which will be synchroized to bangumi, compatible with this repo.
- [bangumi/api](https://github.com/bangumi/api): The api of bangumi.
